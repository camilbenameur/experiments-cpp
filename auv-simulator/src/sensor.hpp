// Sensor interface + simulated environment
//
// The simulator maintains the "true" state of the world (Environment);
// each sensor derives a measurement from it, possibly noisy. The rest of
// the pipeline only sees the interface: adding a sensor changes nothing
// downstream

#ifndef SENSOR_HPP
#define SENSOR_HPP

// True state of the simulated world (the ground truth sensors measure)
struct Environment {
    double depth = 0.0;   // depth in meters, 0 = surface
};

class Sensor {
public:
    virtual ~Sensor() = default;   // destroyed through base pointers
    virtual const char* name() const = 0;
    virtual const char* unit() const = 0;
    virtual double read(const Environment& env, int tick) = 0;
};

#endif
