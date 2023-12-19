#pragma once
#include <optional>

namespace Hal {

struct IP1DataView {
  virtual const char* GetTimestamp() const = 0;
  virtual const char* GetEquipmentId() const = 0;
  virtual double GetEnergyDeliveredTariff1() const = 0;
  virtual double GetEnergyDeliveredTariff2() const = 0;
};

struct IP1DataObserver {
  virtual void OnP1DataReceived(const char* const rawPacket, const IP1DataView& parsedPacket, const char* const errorMsg) = 0;
  virtual ~IP1DataObserver() = default;
};

class IP1Reader {
public:
  virtual void Loop() = 0;
  virtual ~IP1Reader() = default;
};

}
