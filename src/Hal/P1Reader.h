#pragma once
#include <Hal/External/IP1Reader.h>
#include <Hal/External/IPin.h>
#include <Utils/IOutputStream.h>
#include <Utils/NonCopyableAndNonMovable.h>
#include <dsmr.h>

namespace Hal {

class P1Reader : public IP1Reader, Utils::NonCopyableAndNonMovable {
  std::optional<P1Data> _lastReceivedData;
  dsmr::P1Reader _p1Reader;

  P1Reader(Utils::IOutputStream& p1SerialPort, const IPin& p1StreamEnablePin)
      : _p1Reader{&(static_cast<SerialPort&>(p1SerialPort)).Raw(),
                  p1StreamEnablePin.Raw()} {
    _p1Reader.enable(false);
  }

  std::optional<P1Data>& GetLastReceivedData() override { return _lastReceivedData; }

  void Loop() override {
    using DataT = ParsedData<
        /* String */ timestamp,
        /* String */ equipment_id,
        /* FixedValue */ energy_delivered_tariff1,
        /* FixedValue */ energy_delivered_tariff2>;

    _p1Reader.loop();
    String err;

    if (_p1Reader.available()) {
      DEBUG("New packet available");
      DataT data;
      DEBUG("/////");
      DEBUG("%s", _p1Reader.raw().c_str());
      DEBUG("/////");
      if (_p1Reader.parse(&data, &err)) {
        DEBUG("Parsed successfully\n");
        DEBUG("%s\n", data.timestamp.c_str());
        DEBUG("%s\n", data.equipment_id.c_str());
        DEBUG("%f\n", data.energy_delivered_tariff1.val());
        DEBUG("%f\n", data.energy_delivered_tariff2.val());

        _lastReceivedData.emplace(data.timestamp.c_str(),
                                  data.equipment_id.c_str(),
                                  data.energy_delivered_tariff1.val(),
                                  data.energy_delivered_tariff2.val());
      }
    } else {
      // Parser error, print error
      DEBUG("Parsed failed");
      DEBUG("%s", err.c_str());
    }
  }
};

}
