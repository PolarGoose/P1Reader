from collections import namedtuple
import re

header_regex = re.compile(r"/(....)(.*)")
value_regex = re.compile(r"(\d+)-(\d+):(\d+)\.(\d+)\.(\d+)\(([\d\.S]+)(\*.*)?\)")
timestamp_value_regex = re.compile(r"(\d+)-(\d+):(\d+)\.(\d+)\.(\d+)\((\d{12})\)")

ObisCode = namedtuple("ObisCode", [
    'A',  # Medium: (0=abstract objects, 1=electricity, 6=heat, 7=gas, 8=water ...)
    'B',  # Channel
    'C',  # Group: the physical value (current, voltage, energy, level, temperature, ...)
    'D',  # Quantity computation result of specific algorythm
    'E',  # Measurement type defined by groups A to D into individual measurements (e.g. switching ranges)
])


DsmrPacketHeader = namedtuple("DsmrPacketHeader", [
    "Version",
    "Identification",
])


DsmrDataObject = namedtuple("DsmrDataObject", [
    "ObisCode",
    "Value",
    "Unit"
])

DsmrParsedData = namedtuple("DsmrParsedData", [
    "Header",
    "DataObjectsDictionary"
])


def _parse_dsmr_header(first_line:str) -> DsmrPacketHeader:
    match = header_regex.match(first_line)
    return DsmrPacketHeader(match.group(1), match.group(2))


def _parse_dsmr_values(lines: list[str]) -> list[DsmrDataObject]:
    for line in lines[1:]:
        match = value_regex.match(line)
        if match is not None:
            obis_code = ObisCode(int(match.group(1)), int(match.group(2)), int(match.group(3)), int(match.group(4)), int(match.group(5)))
            value = match.group(6)
            unit = ""
            if match.group(7) is not None:
                unit = match.group(7)[1:]
            yield DsmrDataObject(obis_code, value, unit)


def parse_dsmr_packet(packet: str) -> DsmrParsedData:
    lines = packet.splitlines()
    parsed_header = _parse_dsmr_header(lines[0])
    parsed_values_dictionary = {}
    for parsed_value in _parse_dsmr_values(lines[1:]):
        parsed_values_dictionary[parsed_value.ObisCode] = parsed_value
    return DsmrParsedData(parsed_header, parsed_values_dictionary)
