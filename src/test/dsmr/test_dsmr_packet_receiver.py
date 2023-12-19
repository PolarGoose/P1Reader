from app.dsmr.dsmr_packet_receiver import DsmrPacketReceiver


def _assert_packet_is_not_received(packet_data: str, packet_receiver: DsmrPacketReceiver):
    for byte in packet_data:
        packet = packet_receiver.process_byte(byte)
        if packet is not None:
            assert False


def _assert_packet_is_received(packet_data: str, packet_receiver: DsmrPacketReceiver):
    for byte in packet_data:
        packet = packet_receiver.process_byte(byte)
        if packet is not None:
            assert len(packet) == len(packet_data) - 4
            return


def test_receive_correct_packet():
    packet_receiver = DsmrPacketReceiver(1024)
    packet_data = b"/some datadata!02AD"
    _assert_packet_is_received(packet_data, packet_receiver)


def test_receive_packet_with_start_symbol_in_the_middle():
    packet_receiver = DsmrPacketReceiver(1024)
    packet_data = b"/some / datadata!02AD"
    _assert_packet_is_not_received(packet_data, packet_receiver)


def test_receive_packet_bigger_than_buffer():
    packet_receiver = DsmrPacketReceiver(10)
    packet_data = b"/some datadata!02AD"
    _assert_packet_is_not_received(packet_data, packet_receiver)


def test_receive_packet_bigger_than_buffer_2():
    packet_receiver = DsmrPacketReceiver(5)
    packet_data = b"some datadata"
    _assert_packet_is_not_received(packet_data, packet_receiver)


def test_receive_packet_with_incorrect_crc():
    packet_receiver = DsmrPacketReceiver(100)
    packet_data = b"/some datadata!02AE"
    _assert_packet_is_not_received(packet_data, packet_receiver)


def test_receive_packet_with_incorrect_crc_character():
    packet_receiver = DsmrPacketReceiver(100)
    packet_data = b"/some datadata!02RE"
    _assert_packet_is_not_received(packet_data, packet_receiver)


def test_receive_several_packets():
    packet_receiver = DsmrPacketReceiver(1000)
    packet_data = b"gar/bage" + b"/some datadata!02AD" + b"garbage" + b"/incorrect data!02AD" + b"/some datadata!02AD"
    packet_received = 0;

    for byte in packet_data:
        packet = packet_receiver.process_byte(byte)
        if packet is not None:
            packet_received += 1

    assert packet_received == 2
