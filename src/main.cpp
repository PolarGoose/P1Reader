#include "Hal/Esp32Board.h"
#include "Hal/P1DataObserver.h"
#include "Hal/P1Reader.h"
#include "Logging/Logging.h"
#include <optional>

std::optional<Hal::Esp32Board> esp32Board;
std::optional<Hal::P1Reader> reader;
Hal::P1DataObserver p1DataObserver;

void setup() {
  esp32Board.emplace();
  Logging::SetLoggingStream(esp32Board->GetDebugSerialPort());

  reader.emplace(esp32Board->GetP1SerialPort(), esp32Board->GetP1StreamEnablePin(), p1DataObserver);
}

void loop() { reader->Loop(); }
