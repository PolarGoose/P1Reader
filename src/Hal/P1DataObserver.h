#pragma once
#include "Hal/External/IP1Reader.h"
#include "Logging/Logging.h"

namespace Hal {

class P1DataObserver : public Hal::IP1DataObserver, Utils::NonCopyableAndNonMovable {
private:
  virtual void OnP1DataReceived(const char* const rawPacket, const Hal::IP1DataView& parsedPacket, const char* const errorMsg) override {
    SCOPED_LOG

    DEBUG("New packet available");
    DEBUG("/////");
    DEBUG("%s", rawPacket);
    DEBUG("/////");

    if (errorMsg != nullptr) {
      DEBUG("Parsing failed");
      DEBUG("%s", errorMsg);
      return;
    }

    DEBUG("Parsed successfully");
    DEBUG("%s", parsedPacket.GetTimestamp());
    DEBUG("%s", parsedPacket.GetEquipmentId());
    DEBUG("%f", parsedPacket.GetEnergyDeliveredTariff1());
    DEBUG("%f", parsedPacket.GetEnergyDeliveredTariff2());
  }
};

}
