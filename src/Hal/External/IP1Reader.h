#pragma once
#include <optional>
#include <Utils/StaticString.h>

namespace Hal {

struct P1Data {
  Utils::StaticString timestamp;
  Utils::StaticString equipment_id;
  float energy_delivered_tariff1;
  float energy_delivered_tariff2;

  P1Data(std::string_view timestamp, std::string_view equipment_id,
         float energy_delivered_tariff1, float energy_delivered_tariff2)
      : timestamp(timestamp), equipment_id(equipment_id),
        energy_delivered_tariff1(energy_delivered_tariff1),
        energy_delivered_tariff2(energy_delivered_tariff2) {}
};

class IP1Reader {
public:
  virtual std::optional<P1Data>& GetLastReceivedData();
  virtual void Loop();

  virtual ~IP1Reader() {}
};

}
