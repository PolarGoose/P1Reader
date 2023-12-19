#pragma once
#include "Hal/External/IP1Reader.h"
#include "Hal/External/IPin.h"
#include "Hal/External/ISerialPort.h"
#include "Logging/Logging.h"
#include "Utils/NonCopyableAndNonMovable.h"
#include <dsmr.h>

namespace Hal {

using DataT = ParsedData<
    /* String */ timestamp,
    /* String */ equipment_id,
    /* FixedValue */ energy_delivered_tariff1,
    /* FixedValue */ energy_delivered_tariff2>;

class P1DataView : public IP1DataView, Utils::NonCopyableAndNonMovable {
private:
  DataT& _data;
  virtual const char* GetTimestamp() const override { return _data.timestamp.c_str(); }
  virtual const char* GetEquipmentId() const override { return _data.equipment_id.c_str(); }
  virtual double GetEnergyDeliveredTariff1() const override { return _data.energy_delivered_tariff1.val(); }
  virtual double GetEnergyDeliveredTariff2() const override { return _data.energy_delivered_tariff2.val(); }

public:
  P1DataView(DataT& data) : _data{data} {}
  const IP1DataView& GetDataView() const { return *this; }
};

class P1Reader : public IP1Reader, Utils::NonCopyableAndNonMovable {
  dsmr::P1Reader _p1Reader;
  IP1DataObserver& _p1DataObserver;

public:
  P1Reader(ISerialPort& p1SerialPort, const IPin& p1StreamEnablePin, IP1DataObserver& p1DataObserver)
      : _p1Reader{&(static_cast<SerialPort&>(p1SerialPort)).Raw(), p1StreamEnablePin.Raw()}, _p1DataObserver{p1DataObserver} {
    _p1Reader.enable(false);
  }

  void Loop() override {
    _p1Reader.loop();
    if (_p1Reader.available()) {
      DataT data;
      String err;
      const auto rawData{_p1Reader.raw()};
      const auto& success = _p1Reader.parse(&data, &err);
      if (success) {
        _p1DataObserver.OnP1DataReceived(rawData.c_str(), P1DataView(data), nullptr);
        return;
      }
      _p1DataObserver.OnP1DataReceived(_p1Reader.raw().c_str(), P1DataView(data), err.c_str());
    }
  }
};

}
