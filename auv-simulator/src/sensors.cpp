#include "sensors.hpp"

#include <algorithm>

namespace {
constexpr double kGlitchValue = 999.0;   // injected aberrant value
}

DepthSensor::DepthSensor(int glitchPeriod) : glitchPeriod_(glitchPeriod) {}

const char* DepthSensor::name() const { return kDepthSensorName; }
const char* DepthSensor::unit() const { return "m"; }

double DepthSensor::read(const Environment& env, int tick) {
    if (glitchPeriod_ > 0 && tick > 0 && tick % glitchPeriod_ == 0) {
        return kGlitchValue;
    }
    return env.depth;
}

const char* TemperatureSensor::name() const { return kTemperatureSensorName; }
const char* TemperatureSensor::unit() const { return "degC"; }

double TemperatureSensor::read(const Environment& env, int /*tick*/) {
    // 20 degrees at the surface, -0.4 degree per meter, floored at 4
    return std::max(4.0, 20.0 - 0.4 * env.depth);
}
