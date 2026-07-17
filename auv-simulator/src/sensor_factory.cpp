#include "sensor_factory.hpp"

#include <stdexcept>

#include "sensors.hpp"

namespace {
// The depth sensor simulates a periodic spike: the construction point is
// where this kind of configuration belongs
constexpr int kDepthGlitchPeriod = 17;
}

std::unique_ptr<Sensor> makeSensor(const std::string& type) {
    if (type == kDepthSensorName) {
        return std::make_unique<DepthSensor>(kDepthGlitchPeriod);
    }
    if (type == kTemperatureSensorName) {
        return std::make_unique<TemperatureSensor>();
    }
    throw std::invalid_argument("unknown sensor type: " + type);
}
