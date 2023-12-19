# P1Reader
* [P1 DSMR Protocol 5.0.2 Companion Standard](https://www.netbeheernederland.nl/_upload/Files/Slimme_meter_15_a727fce1f1.pdf)
* [ESP32-WROOM-32D Processor Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
* ESP32 Devkit V4
  * [Information about the board. Get Started.](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/hw-reference/esp32/get-started-devkitc.html)
  * [Pinout](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/_images/esp32-devkitC-v4-pinout.png)
  * [Schematics](https://dl.espressif.com/dl/schematics/esp32_devkitc_v4-sch.pdf)
* [Custom P1Reader board schematics](doc/P1ReaderBoardSchematics.pdf)
* How to dump data from the smart meter
  * [P1Port to USB converter](https://www.amazon.nl/gp/product/B07JW75BZ7/)
  * Execute the commands:
    * `mode COM3 BAUD=115200 PARITY=n DATA=8`
    * `copy COM3 /B "data from XS210 smart meter.txt" /B`

# Test bench
<img src="doc/P1ReaderTestBench.svg" width="100%" height="100%"/>

## How to use the test bench
* Connect according to the abovementioned schematics
* Open the the _ESP32 USB-C Debug_ serial port with the parameters: `BAUD=921600`, `PARITY=n`, `DATA=8`
* Execute the commands:
  * `mode COM4 BAUD=115200 PARITY=n DATA=8`
  * `copy "data from XS210 smart meter.txt" /B COM4 /B`
