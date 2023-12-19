import tinyweb
from dsmr_packet_receiving_task import get_last_parsed_packet_data
from dsmr.dsmr_packet_parser import ObisCode
from logging import getLogger

logger = getLogger(__name__)
server = tinyweb.webserver()


@server.route("/power")
async def power_request(request, response):
    await response.start_html()

    packet_data = get_last_parsed_packet_data()

    if packet_data is None:
        await response.send("{}")
        return

    timestamp = packet_data.DataObjectsDictionary[ObisCode(0, 0, 1, 0, 0)].Value
    solar_to_grid = packet_data.DataObjectsDictionary[ObisCode(1, 0, 2, 7, 0)].Value
    grid_to_home = packet_data.DataObjectsDictionary[ObisCode(1, 0, 1, 7, 0)].Value
    await response.send(f'{{ "Timestamp": "{timestamp}", "Solar2Grid": {solar_to_grid}, "Grid2Home": {grid_to_home} }}')


def start_web_server() -> None:
    logger.info("Starting web server")
    server.run(host='0.0.0.0', port=80, loop_forever=False)



