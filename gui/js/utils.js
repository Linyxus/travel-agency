
const generateLine = (road, roadColors, alpha) => {
    let src = road.src;
    let dest = road.dest;
    let srcPos = null;
    let destPos = null;
    for (let j = 0; j < cities.length; j++) {
        if (cities[j].name == src) {
            srcPos = cities[j].center;
        }
        if (cities[j].name == dest) {
            destPos = cities[j].center;
        }
    }
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
        strokeColor: roadColors[road.type],
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

function generatePath(roads, roadColors, alpha) {
    return roads.map(road => generateLine(road, roadColors, alpha));
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
            center: city.center,
            radius: 5.5,
            strokeWeight: 1.5,
            strokeColor: markerColors[city.level],
            strokeOpacity: 0.9,
            fillOpacity: 0,
            zIndex: 10,
            bubble: true,
            cursor: 'pointer',
            clickable: true,
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
                'padding': '0'
            },
            position: city.center
        });

        text.setMap(map);
        map.cityMarkers.push(text);
    }
}

function renderJourney(roads, map) {
    const roadColors = {
        air: '#a0c4ff',
        subway: '#457b9d',
        highway: '#1d3557'
    }
    let paths = generatePath(roads, roadColors, 0.05);
    if (map.journeyPaths === undefined) {
        map.journeyPaths = [];
    }
    map.remove(map.journeyPaths);
    map.journeyPaths = paths;
    map.add(paths);
}
