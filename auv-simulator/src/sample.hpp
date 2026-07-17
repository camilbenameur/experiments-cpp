// A timestamped measurement, as it travels on the telemetry bus
// The raw value is kept next to the filtered one: invaluable when
// diagnosing what the filter corrected (sensor glitch for instance)

#ifndef SAMPLE_HPP
#define SAMPLE_HPP

#include <string>

struct Sample {
    int tick = 0;         // measurement date (simulation step)
    std::string sensor;   // name of the emitting sensor
    std::string unit;     // displayable unit
    double raw = 0.0;     // raw value read from the sensor
    double value = 0.0;   // value after filtering
};

#endif
