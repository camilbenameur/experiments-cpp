# AUV mission simulator (work in progress)

Simulator of the depth-keeping of an autonomous underwater vehicle,
built module by module. Here so far: the statically allocated ring
buffer used on the data path, with its unit tests (Catch2, vendored as
a single header — nothing to install).

```
make test   # builds and runs the unit tests
```

Coming next: simulated sensors and their factory, sliding-window
filters, the mission state machine, the telemetry bus and its
observers.
