#include "Utils/Logging.h"
#include "Hal/Esp32Board.h"
#include <dsmr.h>
#include <optional>

using MyData = ParsedData<
    /* String */ timestamp,
    /* String */ equipment_id,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2>;

std::optional<Hal::Esp32Board> esp32Board;
std::optional<P1Reader> reader;

void setup() {
  SCOPED_LOG;

  esp32Board.emplace();
  reader.emplace(&esp32Board->GetP1SerialPort(), esp32Board->GetP1StreamEnablePinNumber());
  Utils::Logging::SetLoggingStream(esp32Board->GetDebugSerialPort());
}

void loop() {
  reader->loop();

  DEBUG_F("%s", "123");

  if (reader->available()) {
    DEBUG("New packet available");
    MyData data;
    String err;
    DEBUG("/////");
    DEBUG(reader->raw());
    DEBUG("/////");
    if (reader->parse(&data, &err)) {
      DEBUG("Parsed successfully");
      DEBUG(data.timestamp);
      DEBUG(data.equipment_id);
      DEBUG(data.energy_delivered_tariff1.val());
      DEBUG(data.energy_delivered_tariff2.val());
    } else {
      // Parser error, print error
      DEBUG("Parsed failed");
      DEBUG(err);
    }
  }
}
