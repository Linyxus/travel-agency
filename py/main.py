import asyncio
import json
import websockets
from argparse import ArgumentParser
import logging
import os.path as osp

from agency_server import AgencyServer
from travel_agency import LineType, JourneyRecord

logging.basicConfig(level=logging.INFO)


def show_msg(msg):
    if msg['type'] == 'list line':
        return '<list line>'
    elif msg['type'] == 'list city':
        return '<list city>'
    elif msg['type'] == 'list journey':
        return f'<list journey>'
    elif msg['type'] == 'plan':
        return f"<plan: {msg['data']['dep_time']} {msg['data']['src']} -> {msg['data']['dest']}, {msg['data']['strategy']}>"
    elif msg['type'] == 'add journey':
        return f"<add journey: jid={msg['data']['jid']}>"

def city_list():
    return json.dumps({
        'type': 'list city reply',
        'data': [{
            'name': x.name,
            'level': x.level,
            'pos': [x.pos.lat, x.pos.lnt]
        } for x in agency.city_map.cities]
    })


def show_line_type(t):
    m = {
        LineType.Air: 'air',
        LineType.Subway: 'subway',
        LineType.Highway: 'highway'
    }

    return m[t]


def line_list():
    return json.dumps({
        'type': 'list line reply',
        'data': [{
            'from': x.src,
            'to': x.dest,
            'duration': x.duration,
            'dep_time': x.dep_time,
            'type': show_line_type(x.type)
        } for x in agency.city_map.lines]
    })


def journey_list():
    return json.dumps({
        'type': 'list journey reply',
        'data': [conv_journey_record(agency.planed_journey[jid]) for jid in agency.journey_list]
    })


def conv_journey_record(rec: JourneyRecord):
    return {
        'jid': rec.jid,
        'tid': rec.tid,
        'day': rec.day,
        'src': rec.journey.src,
        'dest': rec.journey.dest,
        'length': rec.journey.length,
        'risk': int(rec.journey.risk * 10) / 10,
        'route': rec.journey.route,
        'step_risk': rec.journey.step_risk,
        'dep_time': rec.journey.dep_time,
    }


def plan(info):
    if type(info['time_limit']) is not int:
        return json.dumps({'type': 'plan nak'})

    if info['time_limit'] < 0:
        return json.dumps({'type': 'plan nak'})

    rec = agency.plan(info['tid'], info['src'], info['dest'],
                      info['day'], info['dep_time'], info['time_limit'], info['strategy'])
    if rec is None:
        return json.dumps({'type': 'plan nak'})

    ret = conv_journey_record(rec)
    return json.dumps({
        'type': 'plan reply',
        'data': ret
    })


clients = set()


def register(ws):
    logging.info('connect new client')
    clients.add(ws)


def unregister(ws):
    logging.info('disconnect client')
    clients.remove(ws)


async def list_city_reply(ws):
    logging.info('send response <list city reply>')
    await ws.send(city_list())


async def list_line_reply(ws):
    logging.info('send response <list line reply>')
    await ws.send(line_list())


async def list_journey_reply(ws):
    logging.info('send response <list journey reply>')
    await ws.send(journey_list())


async def plan_reply(ws, info):
    logging.info('send response <plan reply>')
    data = plan(info)
    await ws.send(data)


def add_journey(jid):
    logging.info(f'add journey with jid={jid}')
    agency.journey_list.append(jid)


async def serve(ws, path):
    register(ws)
    try:
        async for msg in ws:
            msg = json.loads(msg)
            logging.info(f'receive request {show_msg(msg)}')
            if msg['type'] == 'list city':
                await list_city_reply(ws)
            if msg['type'] == 'list line':
                await list_line_reply(ws)
            if msg['type'] == 'list journey':
                await list_journey_reply(ws)
            if msg['type'] == 'plan':
                await plan_reply(ws, msg['data'])
            if msg['type'] == 'add journey':
                add_journey(msg['data']['jid'])
                await asyncio.wait([list_journey_reply(x) for x in clients])
    finally:
        unregister(ws)

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--map', type=str,
                        default='../assets/city_map_full.txt')
    args = parser.parse_args()

    agency = AgencyServer(args.map)
    logging.info(
        f'load city map, {len(agency.city_map.cities)} cities, {len(agency.city_map.lines)} lines')

    start_server = websockets.serve(serve, '127.0.0.1', 5678)
    logging.info('start websocket server')
    fileDir = osp.dirname(osp.abspath(__file__))
    pagePath = osp.join(fileDir, '..', 'gui', 'index.html')
    logging.info(f'please open the app at file://{pagePath}')

    # rec = agency.plan('001', 0, 4, 0, 8, None, 'min_risk')
    # print(rec)
    # add_journey(rec.jid)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
