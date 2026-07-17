# AUV mission simulator (work in progress)

Simulator of the depth-keeping of an autonomous underwater vehicle,
built module by module. Here so far: the statically allocated ring
buffer used on the data path, and the simulated sensors with their
factory (`main` will only ever see a `unique_ptr<Sensor>`), each with
unit tests (Catch2, vendored as a single header — nothing to install).

```
make test   # builds and runs the unit tests
```

Coming next: sliding-window filters, the mission state machine, the
telemetry bus and its observers.
