import asyncio
import json
import websockets
from argparse import ArgumentParser

from agency_server import AgencyServer
from travel_agency import LineType

def city_list():
    return json.dumps({
        'type': 'city list',
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
        'type': 'line list',
        'data': [{
            'from': x.src,
            'to': x.dest,
            'duration': x.duration,
            'dep_time': x.dep_time,
            'type': show_line_type(x.type)
        } for x in agency.city_map.lines]
    })

async def serve(ws, path):
    print('new connection')
    await ws.send(city_list())
    await ws.send(line_list())

if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--map', type=str, default='../test/assets/city_map_cn.txt')
    args = parser.parse_args()

    agency = AgencyServer(args.map)

    start_server = websockets.serve(serve, '127.0.0.1', 5678)
    print('server listening at 127.0.0.1:5678')

    asyncio.get_event_loop().run_until_complete(start_server)
    asyncio.get_event_loop().run_forever()
