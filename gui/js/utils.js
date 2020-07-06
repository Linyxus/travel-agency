
const generateLine = (step, roadColors, alpha) => {
    let srcPos = step.from.pos;
    let destPos = step.to.pos;

    let midPos = [(srcPos[0] + destPos[0]) / 2, (srcPos[1] + destPos[1]) / 2]
    let cornerPos = [destPos[0], srcPos[1]]
    let ctlPos = [alpha * cornerPos[0] + (1 - alpha) * midPos[0], alpha * cornerPos[1] + (1 - alpha) * midPos[1]]
    let path = [
        srcPos, [ctlPos, destPos]
    ]

    var line = new AMap.BezierCurve({
        path: path,
        strokeWeight: 5,
        zIndex: 1,
        strokeColor: roadColors[step.type],
        lineCap: 'round',
        lineJoin: 'round',
        showDir: true
    });

    return line;
}

//     let content = [];
//     content.push('<div class="info-card card"><div class="card-content"><p>');
//     content.push('This is a test message');
//     content.push('<p></div></div>');
//     content = content.join('');
//     let caption = src + ' to ' + dest;
//     let info = 'Recent lines'
//     const infoWindow = new AMap.InfoWindow({
//         isCustom: true,
//         content: content,
//         offset: new AMap.Pixel(25, 0)
//     });
//     console.log(content);

//     line.on('mouseover', e => infoWindow.open(map, midPos));
//     line.on('mouseout', e => infoWindow.close())

function generatePath(steps, roadColors, alpha) {
    return steps.map(step => generateLine(step, roadColors, alpha));
}

function renderCities(cities, map) {
    const markerColors = ['#3caea3', '#f6d55c', '#ed553b'];
    if (map.cityMarkers === undefined) {
        map.cityMarkers = [];
    } else {
        map.remove(map.cityMarkers);
    }

    for (let i = 0; i < cities.length; i++) {
        let city = cities[i];
        var circleMarker = new AMap.CircleMarker({
            center: city.pos,
            radius: 5.5,
            strokeWeight: 1.5,
            strokeColor: markerColors[city.level],
            strokeOpacity: 0.9,
            fillOpacity: 0,
            zIndex: 10,
            bubble: true,
            cursor: 'pointer',
            clickable: false,
        })
        circleMarker.setMap(map)
        map.cityMarkers.push(circleMarker);

        var text = new AMap.Text({
            text: city.name,
            anchor: 'bottom-center', // 设置文本标记锚点
            draggable: false,
            cursor: 'pointer',
            angle: 0,
            style: {
                'border-width': '0px',
                'background': 'transparent',
                'text-align': 'center',
                'font-size': '0.8em',
                'font-family': 'Source Code Pro',
                'color': '#457b9d',
                'margin-bottom': '0.5em',
                'padding': '0px'
            },
            position: city.pos
        });

        text.setMap(map);
        map.cityMarkers.push(text);
    }
}

function renderJourney(journey, map) {
    const roadColors = {
        air: '#a0c4ff',
        subway: '#457b9d',
        highway: '#1d3557'
    }
    let paths = generatePath(journey.route, roadColors, 0.05);
    if (map.journeyPaths === undefined) {
        map.journeyPaths = [];
    }
    if (map.travelerMarker === undefined) {
        map.travelerMarker = [];
    }

    map.remove(map.journeyPaths);
    map.journeyPaths = paths;
    map.add(paths);

    let markerPos = [];
    let status = '';
    if (journey.status.state == 'pending') {
        // journey has not started
        markerPos = journey.src.pos;
        status = '即将出发';
    } else if (journey.status.state == 'finished') {
        // journey has finished
        markerPos = journey.dest.pos;
        status = '已抵达';
    } else if (journey.status.state == 'waiting') {
        // waiting
        markerPos = journey.route[journey.status.stage].from.pos;
        status = '正在等候';
    } else {
        // on the way
        let srcPos = journey.route[journey.status.stage].from.pos;
        let destPos = journey.route[journey.status.stage].to.pos;
        markerPos = [(srcPos[0] + destPos[0]) / 2, (srcPos[1] + destPos[1]) / 2];
        status = '在旅途中';
    }
    markerPos = [markerPos[0] + 0.0, markerPos[1]];

    var travelerMarker = new AMap.CircleMarker({
        center: markerPos,
        radius: 10,
        strokeOpacity: 0,
        fillColor: '#48435C',
        fillOpacity: 0.7,
        zIndex: 100,
        bubble: false,
        cursor: 'pointer',
        clickable: true,
    });

    let content = [];
    content.push('<div class="info-card card"><div class="card-content">');
    content.push('<span class="journey-field">旅客编号</span><br/>');
    content.push(journey.tid);
    content.push('<br/>');
    content.push('<span class="journey-field">状态</span><br/>');
    content.push(status);
    content = content.join('');

    if (map.travelerInfoWindow === undefined) {
        const infoWindow = new AMap.InfoWindow({
            isCustom: true,
            content: content,
            offset: new AMap.Pixel(0, -30)
        });

        map.travelerInfoWindow = infoWindow;
    } else {
        map.travelerInfoWindow.setContent(content);
    }

    travelerMarker.on('mouseover', e => map.travelerInfoWindow.open(map, markerPos));
    travelerMarker.on('mouseout', e => map.travelerInfoWindow.close())

    map.remove(map.travelerMarker);
    map.travelerMarker = [travelerMarker];
    map.add([travelerMarker]);
}

function getPassedTime(day, now, dep_day, dep_time) {
    return (day - dep_day) * 24 + (now - dep_time);
}

function timeBetween(t1, t2) {
    return (t2 - t1 + 24) % 24;
}

function getJourneyStatus(dep_time, route, passed) {
    if (passed < 0) {
        return {
            stage: 0,
            state: 'pending'
        }
    }

    var t = 0;
    var now = dep_time;
    for (var i = 0; i < route.length; i++) {
        t += timeBetween(now, route[i].dep_time);
        if (t > passed) {
            return {
                stage: i,
                state: 'waiting'
            }
        }

        t += route[i].duration;
        if (t > passed) {
            return {
                stage: i,
                state: 1 - (t - passed) / route[i].duration
            }
        }
        now = route[i].arrive_time;
    }
    return {
        stage: route.length,
        state: 'finished'
    }
}

function isNumber(n) { return !isNaN(parseFloat(n)) && !isNaN(n - 0) }

const validateForm = form => {
    if (form.src === '') {
        return [false, '未选择出发地'];
    }
    if (form.dest === '') {
        return [false, '未选择目的地'];
    }
    if (form.dep_time === '') {
        return [false, '未选择出发时间'];
    }
    if (!form.strategy) {
        return [false, '未选择旅行策略'];
    }
    if (form.src == form.dest) {
        return [false, '出发地与目的地不能相同'];
    }

    if (!isNumber(form.time_limit)) {
        return [false, '无效的整数格式'];
    }

    return [true, ''];
}

