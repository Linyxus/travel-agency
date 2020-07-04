const cities = [
    {
        center: [116.40, 39.90],
        level: 2,
        name: '北京'
    },
    {
        center: [121.26, 31.12],
        level: 1,
        name: '上海'
    },
    {
        center: [106.33, 29.33],
        level: 0,
        name: '重庆'
    },
    {
        center: [112.55, 28.12],
        level: 0,
        name: '长沙'
    },
    {
        center: [113.23333, 23.16667],
        level: 1,
        name: '广州'
    },
    {
        center: [118.78333, 32.05000],
        level: 0,
        name: '南京'
    },
    {
        center: [114.31667, 30.51667],
        level: 2,
        name: '武汉'
    },
    {
        center: [106.26667, 38.46667],
        level: 0,
        name: '银川'
    },
    {
        center: [108.95000, 34.26667],
        level: 1,
        name: '西安'
    },
    {
        center: [120.33333, 36.06667],
        level: 0,
        name: '青岛'
    },
    {
        center: [102.73333, 25.05000],
        level: 1,
        name: '昆明'
    },
    {
        center: [120.15358, 30.287458],
        level: 2,
        name: '杭州'
    },
    {
        center: [108.320007, 22.82402],
        level: 0,
        name: '南宁'
    },
    {
        center: [91.11450, 29.644150],
        level: 1,
        name: "拉萨"
    }
]

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

var data = {
    cities: [],
    journeys: journeys,
    selectedJourney: 0,

    running: true,
    addingJourney: false,
    
    newJourney: {
        src: '',
        dest: ''
    },

    typeNames: {
        air: '飞机',
        subway: '火车',
        highway: '客车'
    },
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
            this.newJourney.src = '';
            this.newJourney.dest = '';
        }
    },
    computed: {
        addJourneyCardClass: function() {
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
        }
    }
})

// initialzie Gaode map
var map = new AMap.Map('map', {
    mapStyle: 'amap://styles/whitesmoke',
    zoom: 5.3,
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
        map.journeyPaths = [];
    } else {
        for (let i = 0; i < data.journeys.length; i++) {
            let journey = data.journeys[i];
            if (journey.id == selected) {
                let roads = journey.path;
                renderJourney(roads, map);
            }
        }
    }
})

vm.$watch('addingJourney', function (adding, old) {
    if (adding) {
        this.addJourneyModal.open();
    } else {
        this.addJourneyModal.close();
    }
})

data.cities = cities;

// initialize Materialize
document.addEventListener('DOMContentLoaded', function () {
    M.AutoInit();
    let elem = document.querySelector('#journey-list');
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
