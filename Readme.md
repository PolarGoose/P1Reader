# P1Reader
The custom ESP32-based board to read the data from the smart electricity meters via the P1 port.

# DSMR protocol and P1 port description
* [Overview of DSMR resources and standards](https://github.com/energietransitie/dsmr-info)
* [P1 DSMR Protocol 5.0.2 Companion Standard](https://www.netbeheernederland.nl/publicatie/dsmr-502-p1-companion-standard)

# Board
[Schematics](res/P1ReaderBoardSchematics.pdf)<br>
[ESP32-WROOM-32D Processor Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)<br>
<img src="res/P1ReaderBoard_front.png" width="50%"/>
<img src="res/P1ReaderBoard_back.png" width="50%"/>

# Test bench
ESP32 Devkit V4
* [Information about the board. Get Started.](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
* [Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/_images/esp32_devkitC_v4_pinlayout.png)
* [Schematics](https://dl.espressif.com/dl/schematics/esp32_devkitc_v4-sch.pdf)
<img src="res/P1ReaderTestBench.svg" width="100%" height="100%"/>

## How to use the test bench
* Unzip the DSMR data file `res/data from XS210 smart meter.zip`
* Execute the commands to feed the DSMR data to the board:
  * `mode COM4 BAUD=115200 PARITY=n DATA=8`
  * `copy "data from XS210 smart meter.txt" /B COM4 /B`

## How to dump data from the smart meter
* Use [P1Port to USB converter](https://www.amazon.nl/gp/product/B07JW75BZ7/)
* Execute the commands:
  * `mode COM4 BAUD=115200 PARITY=n DATA=8`
  * `copy COM4 /B "data from XS210 smart meter.txt" /B`

# Micropython
[Micropython ESP32](https://docs.micropython.org/en/latest/esp32/tutorial/intro.html)

# How to upload the FW and the code to the board
Run the scripts from PowerShell:
```
scripts/UploadMicropythonFwToBoard.ps1 -comPort COM3
scripts/UploadSourceCode_RestartBoard_OpenRepl.ps1 -comPort COM3 -ssid wifi_name -pass wifi_password
```
