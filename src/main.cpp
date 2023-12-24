#include "Logging/Logging.h"
#include "Hal/Esp32Board.h"
#include <dsmr.h>
#include <optional>
#include "Hal/P1Reader.h"

using MyData = ParsedData<
    /* String */ timestamp,
    /* String */ equipment_id,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2>;

std::optional<Hal::Esp32Board> esp32Board;
std::optional<P1Reader> reader;

void setup() {
  SCOPED_LOG

  esp32Board.emplace();
  reader.emplace(&esp32Board->GetP1SerialPort().Raw(), esp32Board->GetP1StreamEnablePinNumber());
  Logging::SetLoggingStream(esp32Board->GetDebugSerialPort());
}

void loop() {
  reader->loop();

  if (reader->available()) {
    DEBUG("New packet available");
    MyData data;
    String err;
    DEBUG("/////");
    DEBUG("%s", reader->raw().c_str());
    DEBUG("/////");
    if (reader->parse(&data, &err)) {
      DEBUG("Parsed successfully\n");
      DEBUG("%s\n", data.timestamp.c_str());
      DEBUG("%s\n", data.equipment_id.c_str());
      DEBUG("%f\n", data.energy_delivered_tariff1.val());
      DEBUG("%f\n", data.energy_delivered_tariff2.val());
    } else {
      // Parser error, print error
      DEBUG("Parsed failed");
      DEBUG("%s", err.c_str());
    }
  }
}
