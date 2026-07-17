// Concrete sensors of the simulator

#ifndef SENSORS_HPP
#define SENSORS_HPP

#include "sensor.hpp"

// Canonical names: avoids magic strings scattered across the code
inline constexpr const char* kDepthSensorName = "depth";
inline constexpr const char* kTemperatureSensorName = "temperature";

// Depth sensor. A real one would derive depth from a pressure sensor
// (~1 bar / 10 m); here we read the simulated world directly
// glitchPeriod > 0: every glitchPeriod readings, the raw value goes
// aberrant (simulated EMI-like spike). Absorbing it is the job of the
// median filter downstream, not of a hypothetical perfect sensor
class DepthSensor : public Sensor {
public:
    explicit DepthSensor(int glitchPeriod = 0);
    const char* name() const override;
    const char* unit() const override;
    double read(const Environment& env, int tick) override;

private:
    int glitchPeriod_;
};

// Water temperature: decreases with depth (very simplified thermocline),
// floored at 4 degrees in deep water
class TemperatureSensor : public Sensor {
public:
    const char* name() const override;
    const char* unit() const override;
    double read(const Environment& env, int tick) override;
};

#endif
