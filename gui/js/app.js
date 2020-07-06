Number.prototype.pad = function (size) {
    var s = String(this);
    while (s.length < (size || 2)) { s = "0" + s; }
    return s;
}

const journeys = [
    {
        traveler: '001',
        id: 'journey-0',
        src: '重庆',
        dest: '上海',
        duration: 10,
        risk: 0.1,
        path: [
            {
                src: '重庆',
                dest: '昆明',
                type: 'subway',
                risk: 0.9,
                duration: 1,
                active: false
            },
            {
                src: '昆明',
                dest: '武汉',
                type: 'highway',
                risk: 0.1,
                duration: 2,
                active: true
            },
            {
                src: '武汉',
                dest: '上海',
                type: 'air',
                risk: 0.1,
                duration: 10,
                active: false
            },
        ]
    },
    {
        traveler: '002',
        id: 'journey-1',
        src: '上海',
        dest: '青岛',
        duration: 5,
        risk: 2.1,
        path: [
            {
                src: '上海',
                dest: '青岛',
                type: 'air',
                active: true
            },
            {
                src: '青岛',
                dest: '北京',
                type: 'subway',
                active: false
            },
        ]
    },
]

let mkTickTimer = () => setInterval(() => {
    vm.now.time = vm.now.time + 1;
    if (vm.now.time >= 24) {
        vm.now.time = vm.now.time - 24;
        vm.now.day = vm.now.day + 1;
    }
}, 1000);

var data = {
    cities: [],
    lines: [],
    journeys: [],
    selectedJourney: 0,
    now: {
        day: 0,
        time: 0
    },
    tickTimer: mkTickTimer(),

    running: true,
    addingJourney: false,

    nextTid: 1,

    newJourney: {
        src: '',
        dest: '',
        dep_time: '',
        strategy: '',
        time_limit: 0,
    },

    typeNames: {
        air: '飞机',
        subway: '火车',
        highway: '客车'
    },

    classOfStepStatus: {
        pending: 'journey-step',
        finished: 'journey-step journey-step-finished',
        active: 'journey-step journey-step-active'
    },

    cachedJourney: {
        journey: null,
        form: null,
        error: false,
    },

    requestingCache: false,

    socket: null,
}

// initialzie Vue
var vm = new Vue({
    el: '#sidebar',
    data: data,
    methods: {
        handleAddButtonClick: function (e) {
            this.running = false;
            this.addingJourney = true;
        },
        handleAddCancelButtonClick: function (e) {
            this.running = true;
            this.addingJourney = false;
        },
        handleConfirmButtonClick: function (e) {
            if (this.cachedJourney.error) {
                M.toast({ html: '没有可行的行程' });
                return;
            }

            const ret = validateForm(this.newJourney);
            if (!ret[0]) {
                M.toast({ html: ret[1] });
            } else {
                this.socket.send(JSON.stringify({
                    type: 'add journey',
                    data: {
                        jid: this.cachedJourney.journey.jid
                    }
                }));
                this.nextTid = this.nextTid + 1;
                this.running = true;
                this.addingJourney = false;
            }
        },

        // cache-related
        requestJourney: function () {
            const payload = {
                tid: this.nextTid.pad(3),
                src: this.newJourney.src,
                dest: this.newJourney.dest,
                day: this.now.day,
                dep_time: this.newJourney.dep_time,
                time_limit: this.newJourney.time_limit,
                strategy: this.newJourney.strategy
            };

            if (payload.dep_time < this.now.time) {
                payload.day = payload.day + 1;
            }

            this.socket.send(JSON.stringify({
                type: 'plan',
                data: payload,
            }));

            this.requestingCache = true;
        },
    },
    computed: {
        addJourneyCardClass: function () {
            if (this.addingJourney) {
                return "card add-journey-card scale-transition scale-out scale-in";
            } else {
                return "card add-journey-card scale-transition scale-out";
            }
        },
        addButtonClass: function () {
            if (this.addingJourney) {
                return "btn-floating btn-large blue waves-effect waves-light scale-transition scale-out";
            } else {
                return "btn-floating btn-large blue waves-effect waves-light scale-transition scale-out scale-in";
            }
        },
        processedCachedJourney: function () {
            if (this.cacheStatus == 'okay') {
                return [this.cachedJourney.journey].map(j => {
                    let ret = Object.assign({}, j);
                    ret.route = ret.route.map((r, i) => {
                        let p = Object.assign({}, this.lines[r]);
                        p.active = false;
                        p.from = this.cities[p.from];
                        p.to = this.cities[p.to];
                        p.risk = ret.step_risk[i];
                        p.arrive_time = (p.dep_time + p.duration) % 24;

                        return p;
                    });
                    ret.src = this.cities[ret.src];
                    ret.dest = this.cities[ret.dest];
                    ret.arrive_time = (ret.dep_time + ret.length) % 24;
                    ret.arrive_day = ret.day + Math.floor((ret.dep_time + ret.length) / 24);

                    ret.status = getJourneyStatus(
                        ret.dep_time, ret.route, getPassedTime(this.now.day, this.now.time, ret.day, ret.dep_time));

                    ret.isPending = ret.status.state == 'pending';
                    ret.isFinished = ret.status.state == 'finished';
                    ret.isOngoing = !(ret.isPending || ret.isFinished);

                    for (let i = 0; i < ret.route.length; i++) {
                        if (i < ret.status.stage) {
                            ret.route[i].status = 'finished';
                        }

                        if (i == ret.status.stage) {
                            if (ret.status.state == 'waiting' || ret.status.state == 'pending')
                                ret.route[i].status = 'pending';
                            else
                                ret.route[i].status = 'active';
                        }

                        if (i > ret.status.stage) {
                            ret.route[i].status = 'pending';
                        }
                    }

                    return ret;
                })[0];
            }
        },
        processedJourneys: function () {
            return this.journeys.map(j => {
                let ret = Object.assign({}, j);
                ret.route = ret.route.map((r, i) => {
                    let p = Object.assign({}, this.lines[r]);
                    p.active = false;
                    p.from = this.cities[p.from];
                    p.to = this.cities[p.to];
                    p.risk = ret.step_risk[i];
                    p.arrive_time = (p.dep_time + p.duration) % 24;

                    return p;
                });
                ret.src = this.cities[ret.src];
                ret.dest = this.cities[ret.dest];
                ret.arrive_time = (ret.dep_time + ret.length) % 24;
                ret.arrive_day = ret.day + Math.floor((ret.dep_time + ret.length) / 24);

                ret.status = getJourneyStatus(
                    ret.dep_time, ret.route, getPassedTime(this.now.day, this.now.time, ret.day, ret.dep_time));

                ret.isPending = ret.status.state == 'pending';
                ret.isFinished = ret.status.state == 'finished';
                ret.isOngoing = !(ret.isPending || ret.isFinished);

                for (let i = 0; i < ret.route.length; i++) {
                    if (i < ret.status.stage) {
                        ret.route[i].status = 'finished';
                    }

                    if (i == ret.status.stage) {
                        if (ret.status.state == 'waiting' || ret.status.state == 'pending')
                            ret.route[i].status = 'pending';
                        else
                            ret.route[i].status = 'active';
                    }

                    if (i > ret.status.stage) {
                        ret.route[i].status = 'pending';
                    }
                }

                return ret;
            });
        },
        depTimeOptions: function () {
            var ret = [];
            for (let i = 0; i < 24; i++) {
                let label = null;
                if (i + this.now.time < 24) {
                    label = ((i + this.now.time) % 24).pad(2) + ':00';
                } else {
                    label = ((i + this.now.time) % 24).pad(2) + ':00 (明天)';
                }
                ret.push({
                    value: (i + this.now.time) % 24,
                    label: label
                });
            }

            return ret;
        },
        canCache: function () {
            return this.addingJourney && validateForm(this.newJourney)[0];
        },
        cacheStatus: function () {
            if (this.requestingCache) {
                return 'requesting';
            }

            if (!this.cachedJourney.form) {
                return 'empty';
            }

            let eq = this.cachedJourney.form.src == this.newJourney.src
                && this.cachedJourney.form.dest == this.newJourney.dest
                && this.cachedJourney.form.dep_time == this.newJourney.dep_time
                && this.cachedJourney.form.time_limit == this.newJourney.time_limit
                && this.cachedJourney.form.strategy == this.newJourney.strategy;

            if (!eq) {
                return 'outdated';
            } else {
                return 'okay';
            }
        },
        needCache: function () {
            return !this.requestingCache && this.canCache && ((this.cacheStatus == 'empty' && !this.cachedJourney.error) || this.cacheStatus == 'outdated');
        }
    }
})

// initialzie Gaode map
var map = new AMap.Map('map', {
    mapStyle: 'amap://styles/whitesmoke',
    zoom: 5,
    center: [106.5, 32],
    viewMode: '3D'
});

map.setFeatures(['road', 'bg']);

vm.$watch('cities', function (cities, old) {
    renderCities(cities, map);
});

vm.$watch('selectedJourney', function (selected, old) {
    if (!selected) {
        map.remove(map.journeyPaths);
        map.remove(map.travelerMarker);
        map.journeyPaths = [];
        map.travelerMarker = [];
    } else {
        for (let i = 0; i < vm.processedJourneys.length; i++) {
            let journey = vm.processedJourneys[i];
            if (journey.jid == selected) {
                renderJourney(journey, map);
            }
        }
    }
});

vm.$watch('processedJourneys', function (journeys, old) {
    if (this.selectedJourney) {
        for (let i = 0; i < journeys.length; i++) {
            let journey = journeys[i];
            if (journey.jid == vm.selectedJourney) {
                renderJourney(journey, map);
            }
        }
    }

});

vm.$watch('addingJourney', function (adding, old) {
    if (adding) {
        this.newJourney.src = '';
        this.newJourney.dest = '';
        this.newJourney.dep_time = '';
        this.newJourney.strategy = '';
        this.newJourney.time_limit = 0;

        this.cachedJourney.form = null;
        this.cachedJourney.journey = null;
        this.cachedJourney.error = false;

        let elems = document.querySelectorAll('select');
        M.FormSelect.init(elems, {});
        this.addJourneyModal.open();
    } else {
        this.addJourneyModal.close();
    }
});

vm.$watch('running', function (running, old) {
    if (!running) {
        clearInterval(this.tickTimer);
    } else {
        this.tickTimer = mkTickTimer();
    }
});

vm.$watch('cities', function (cities, old) {
    let elems = document.querySelectorAll('select');
    M.FormSelect.init(elems, {});
});

vm.$watch('needCache', function (b, old) {
    if (b) {
        console.log('updating cache ...')
        this.cachedJourney.form = Object.assign({}, this.newJourney);
        this.requestJourney();
    }
});

// initialize Materialize
document.addEventListener('DOMContentLoaded', function () {
    M.AutoInit();
    let elem = document.querySelectorAll('#journey-list');
    M.Collapsible.init(elem, {
        onOpenStart: e => {
            data.selectedJourney = e.id;
        },
        onCloseStart: e => {
            if (e.id == data.selectedJourney) {
                data.selectedJourney = 0;
            }
        }
    });

    elem = document.querySelector('#modal_add-journey');
    var addJourneyModal = M.Modal.init(elem, {
        dismissible: false
    });
    vm.addJourneyModal = addJourneyModal;
});

let socket = new WebSocket('ws://127.0.0.1:5678');

socket.onopen = e => {
    console.log('Websocket connected')
    socket.send(JSON.stringify({ type: 'list city' }));
    socket.send(JSON.stringify({ type: 'list line' }));
    socket.send(JSON.stringify({ type: 'list journey' }));
}

socket.onmessage = e => {
    let msg = JSON.parse(e.data);
    console.log(msg);
    if (msg.type == 'list city reply') {
        vm.cities = msg.data;
    }
    if (msg.type == 'list line reply') {
        vm.lines = msg.data;
    }
    if (msg.type == 'list journey reply') {
        vm.journeys = msg.data;
    }
    if (msg.type == 'plan reply') {
        vm.cachedJourney.error = false;
        vm.cachedJourney.journey = msg.data;
        vm.requestingCache = false;
    }
    if (msg.type == 'plan nak') {
        vm.cachedJourney.error = true;
        vm.requestingCache = false;
    }
}

vm.socket = socket;
