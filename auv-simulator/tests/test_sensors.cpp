#include "catch.hpp"

#include <stdexcept>
#include <string>

#include "../src/sensor_factory.hpp"
#include "../src/sensors.hpp"

TEST_CASE("Factory: builds the right sensor from its name") {
    auto depth = makeSensor(kDepthSensorName);
    REQUIRE(std::string(depth->name()) == kDepthSensorName);
    REQUIRE(std::string(depth->unit()) == "m");

    auto temp = makeSensor(kTemperatureSensorName);
    REQUIRE(std::string(temp->name()) == kTemperatureSensorName);
}

TEST_CASE("Factory: unknown type -> exception") {
    REQUIRE_THROWS_AS(makeSensor("sonar"), std::invalid_argument);
}

TEST_CASE("DepthSensor: reads the simulated world depth") {
    DepthSensor sensor;   // no glitch
    Environment env;
    env.depth = 12.5;
    REQUIRE(sensor.read(env, 0) == Approx(12.5));
    REQUIRE(sensor.read(env, 17) == Approx(12.5));
}

TEST_CASE("DepthSensor: periodic glitch on the raw value") {
    DepthSensor sensor(5);
    Environment env;
    env.depth = 12.5;

    REQUIRE(sensor.read(env, 4) == Approx(12.5));
    REQUIRE(sensor.read(env, 5) > 100.0);           // aberration at tick 5
    REQUIRE(sensor.read(env, 6) == Approx(12.5));
    REQUIRE(sensor.read(env, 0) == Approx(12.5));   // no glitch at t=0
}

TEST_CASE("TemperatureSensor: decreases with depth, floored at 4 degrees") {
    TemperatureSensor sensor;
    Environment env;

    env.depth = 0.0;
    double surface = sensor.read(env, 0);

    env.depth = 10.0;
    double at10m = sensor.read(env, 0);
    REQUIRE(at10m < surface);

    env.depth = 500.0;
    REQUIRE(sensor.read(env, 0) == Approx(4.0));
}
