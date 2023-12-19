from os import statvfs
from gc import mem_free, mem_alloc


def get_total_file_system_size_in_kb():
    stat = statvfs("/")
    return stat[0] * stat[2] / 1024


def get_free_file_system_size_in_kb():
    stat = statvfs("/")
    return stat[0] * stat[3] / 1024


def get_free_ram_size_in_kb():
    return mem_free() / 1024


def get_used_ram_size_in_kb():
    return mem_alloc() / 1024


def get_total_ram_size_in_kb():
    return (mem_alloc() + mem_free()) / 1024
