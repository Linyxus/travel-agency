import asyncio
import json
import websockets
from argparse import ArgumentParser
import logging

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
        'risk': rec.journey.risk,
        'route': rec.journey.route,
        'dep_time': rec.journey.dep_time,
    }


def plan(info):
    rec = agency.plan(info['tid'], info['src'], info['dest'],
                      info['day'], info['dep_time'], info['strategy'])
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
                await plan_reply(msg['data'])
            if msg['type'] == 'add journey':
                add_journey(msg['data']['jid'])
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

    rec = agency.plan('001', 0, 4, 0, 8, 'min_risk')
    add_journey(rec.jid)

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
