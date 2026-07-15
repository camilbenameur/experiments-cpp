// Polymorphism, abstract class, virtual destructor
// Build: g++ -std=c++17 -Wall -Wextra polymorphism.cpp -o prog && ./prog
//
// The indispensable qualifier on ~Robot: `virtual`. Without it,
// destroying through unique_ptr<Robot> would never call ~Drone / ~Rover:
// undefined behavior, and derived resources never released

#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Robot {
public:
    explicit Robot(std::string id) : id_(std::move(id)) {}
    virtual ~Robot() = default;              // <-- THE point of this exercise
    virtual void move(double meters) = 0;    // pure virtual -> abstract class
    const std::string& id() const { return id_; }
private:
    std::string id_;
};

class Drone : public Robot {
public:
    explicit Drone(std::string id) : Robot(std::move(id)) {}
    ~Drone() override { std::cout << "[destroying Drone " << id() << "]\n"; }
    void move(double m) override {
        std::cout << id() << " moves " << m << "m (drone: rotors)\n";
    }
};

class Rover : public Robot {
public:
    explicit Rover(std::string id) : Robot(std::move(id)) {}
    ~Rover() override { std::cout << "[destroying Rover " << id() << "]\n"; }
    void move(double m) override {
        std::cout << id() << " moves " << m << "m (rover: tracks)\n";
    }
};

int main() {
    std::vector<std::unique_ptr<Robot>> units;
    units.push_back(std::make_unique<Drone>("UAV-1"));
    units.push_back(std::make_unique<Rover>("RV-2"));

    for (const auto& u : units) {
        u->move(100.0);
    }
    // The vector destroys its unique_ptrs here: thanks to the virtual
    // destructor, every derived destructor is called
    return 0;
}
