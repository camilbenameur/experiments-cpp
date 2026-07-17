// Sensor factory (Factory pattern)
//
// Centralizes sensor creation and configuration: the caller asks for a
// type by name (as a config file would) and only ever handles the Sensor
// interface. Returning a unique_ptr makes the ownership transfer
// explicit: the caller owns the sensor, destruction is guaranteed (RAII)

#ifndef SENSOR_FACTORY_HPP
#define SENSOR_FACTORY_HPP

#include <memory>
#include <string>

#include "sensor.hpp"

// Throws std::invalid_argument for an unknown type
std::unique_ptr<Sensor> makeSensor(const std::string& type);

#endif
