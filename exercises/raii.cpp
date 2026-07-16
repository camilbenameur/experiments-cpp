// From new/delete to RAII and unique_ptr
// Build: g++ -std=c++20 -Wall -Wextra raii.cpp -o prog && ./prog
//
// The initial version managed memory by hand and had two leaks:
//   - a guaranteed leak: `session` (new without delete)
//   - a conditional leak: in processRun, if the exception fired, the
//     final delete was never reached
// Rewritten with unique_ptr: no visible new/delete left, both leaks are
// gone by construction. The unique_ptr destructor runs during stack
// unwinding: "remembering the delete" cannot cover every exit path;
// a destructor can

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

class DataLog {
public:
    explicit DataLog(std::string name) : name_(std::move(name)) {
        std::cout << "[open] " << name_ << "\n";
    }
    ~DataLog() { std::cout << "[close] " << name_ << "\n"; }
    // string_view: write only reads the text, no copy, and accepts
    // string literals as well as std::string
    void write(std::string_view line) { std::cout << name_ << ": " << line << "\n"; }
private:
    std::string name_;
};

void processRun(bool simulateFailure) {
    auto log = std::make_unique<DataLog>("run-042");
    log->write("measurement run started");

    if (simulateFailure) {
        throw std::runtime_error("signal lost");
        // unique_ptr destroyed during unwinding -> [close] run-042 guaranteed
    }

    log->write("run finished");
}   // automatic destruction on the nominal path too

int main() {
    auto session = std::make_unique<DataLog>("session");
    session->write("session opened");

    try {
        processRun(true);
    } catch (const std::exception& e) {
        std::cout << "error: " << e.what() << "\n";
    }
    return 0;
}   // [close] session guaranteed
