import os


try:
    import machine
except ImportError:
    print('This script can only be run on a MicroPython device')
    exit(1)


def delete_all_files_and_directories(directory):
    for file_info in os.ilistdir(directory):
        file_name, file_type = file_info[0:2]
        if file_name == 'boot.py':
            continue
        file_full_name = f"{directory}/{file_name}"
        print(f"Remove {file_full_name}")
        if file_type == 0x8000: # file
            os.remove(file_full_name)
        else:
            delete_all_files_and_directories(file_full_name)
            os.rmdir(file_full_name)


delete_all_files_and_directories("/")
