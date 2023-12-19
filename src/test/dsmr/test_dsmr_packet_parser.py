from app.dsmr.dsmr_packet_parser import parse_dsmr_packet, ObisCode, DsmrDataObject


def test_parser():
    packet = "/Ene5\\XS210 ESMR 5.0\r\n" \
             "\r\n" \
             "1-3:0.2.8(50)\r\n" \
             "0-0:1.0.0(231017090442S)\r\n" \
             "0-0:96.1.1(4530303437303030303434363636353138)\r\n" \
             "1-0:1.8.1(008243.448*kWh)\r\n" \
             "1-0:1.8.2(010196.219*kWh)\r\n" \
             "1-0:2.8.1(000000.005*kWh)\r\n" \
             "1-0:2.8.2(000000.000*kWh)\r\n" \
             "0-0:96.14.0(0002)\r\n" \
             "1-0:1.7.0(03.229*kW)\r\n" \
             "1-0:2.7.0(00.000*kW)\r\n" \
             "0-0:96.7.21(00103)\r\n" \
             "0-0:96.7.9(00004)\r\n" \
             "1-0:99.97.0(3)(0-0:96.7.19)(230608111028S)(0000000500*s)(220127110938W)(0000007650*s)(200331155338S)(0000004144*s)\r\n" \
             "1-0:32.32.0(00009)\r\n" \
             "1-0:32.36.0(00000)\r\n" \
             "0-0:96.13.0()\r\n" \
             "1-0:32.7.0(222.0*V)\r\n" \
             "1-0:31.7.0(014*A)\r\n" \
             "1-0:21.7.0(03.229*kW)\r\n" \
             "1-0:22.7.0(00.000*kW)\r\n" \
             "0-1:24.1.0(003)\r\n" \
             "0-1:96.1.0(4730303539303033393036323731353139)\r\n" \
             "0-1:24.2.1(231017090000S)(04547.595*m3)\r\n" \
             "!E164\r\n";

    parsed_data = parse_dsmr_packet(packet)

    assert parsed_data.Header.Version == "Ene5"
    assert parsed_data.Header.Identification == "\\XS210 ESMR 5.0"

    assert parsed_data.DataObjectsDictionary[ObisCode(1, 3, 0, 2, 8)] == DsmrDataObject(ObisCode(1, 3, 0, 2, 8), "50", "")
    assert parsed_data.DataObjectsDictionary[ObisCode(0, 0, 1, 0, 0)] == DsmrDataObject(ObisCode(0, 0, 1, 0, 0), "231017090442S", "")
    assert parsed_data.DataObjectsDictionary[ObisCode(0, 0, 96, 1, 1)] == DsmrDataObject(ObisCode(0, 0, 96, 1, 1), "4530303437303030303434363636353138", "")
    assert parsed_data.DataObjectsDictionary[ObisCode(1, 0, 1, 8, 1)] == DsmrDataObject(ObisCode(1, 0, 1, 8, 1), "008243.448", "kWh")
