# ATLAS Simulators

This directory contains hardware and environment simulation tools for testing the ATLAS without physical drones or radio equipment.

## Overview

The ATLAS simulator ecosystem enables:
- **Development without hardware**: Test ATLAS functionality without physical drones
- **Scalable testing**: Simulate fleets of 25+ drones on a single machine
- **Scenario testing**: Reproduce specific conditions and edge cases
- **Integration testing**: Test complete system workflows safely
- **Training**: Operator training in a controlled environment

## Structure

```
simulators/
├── drone_simulator/           # Virtual drone fleet simulation
│   ├── src/                  # Drone simulator implementation
│   ├── models/               # Different drone type models
│   ├── behaviors/            # Autonomous behavior implementations
│   ├── physics/              # Flight dynamics and physics
│   └── README.md            # Drone simulator documentation
├── radio_simulator/          # GNU Radio and RF simulation
│   ├── src/                  # Radio communication simulation
│   ├── protocols/            # Protocol implementations (MAVLink, etc.)
│   ├── rf_models/            # RF propagation and interference models
│   └── README.md            # Radio simulator documentation
├── environment_simulator/    # Environmental conditions simulation
│   ├── weather/              # Weather simulation (wind, rain, etc.)
│   ├── terrain/              # Terrain and obstacle simulation
│   ├── interference/         # RF interference simulation
│   └── README.md            # Environment simulator documentation
├── scenarios/                # Pre-defined test scenarios
│   ├── search_rescue/        # Search and rescue scenarios
│   ├── failure_modes/        # Equipment failure scenarios
│   ├── stress_tests/         # Performance and load test scenarios
│   └── training/             # Operator training scenarios
├── integration/              # Simulator integration with ATLAS
│   ├── atlas_bridge.py       # Bridge between ATLAS and simulators
│   ├── scenario_runner.py    # Automated scenario execution
│   └── metrics_collector.py  # Performance and behavior metrics
└── tools/                    # Simulator utilities and tools
    ├── scenario_editor.py    # GUI for creating/editing scenarios
    ├── visualization.py      # Real-time simulation visualization
    └── analysis.py           # Post-simulation analysis tools
```

## Best Practices

### **Scenario Design**
1. **Start simple**: Begin with basic scenarios and add complexity
2. **Realistic parameters**: Use real-world values for physics and timing
3. **Clear objectives**: Define measurable success criteria
4. **Edge cases**: Include failure modes and edge conditions
5. **Documentation**: Document scenario purpose and expected outcomes

### **Performance Optimization**
1. **Appropriate fidelity**: Match simulation detail to testing needs
2. **Efficient updates**: Only update necessary simulation components
3. **Memory management**: Clean up unused simulation objects
4. **Parallel processing**: Utilize multiple CPU cores when possible
5. **Caching**: Cache expensive calculations when possible

### **Integration Testing**
1. **Realistic timing**: Match real hardware timing characteristics
2. **Error injection**: Test ATLAS error handling with simulated failures
3. **Load testing**: Use simulation for scalability testing
4. **Continuous validation**: Regularly validate simulation accuracy
5. **Automation**: Automate regression testing with simulation

For detailed simulator documentation, see:
- [Drone Simulator Guide](drone_simulator/README.md)
- [Radio Simulator Guide](radio_simulator/README.md)
- [Environment Simulator Guide](environment_simulator/README.md)
- [Scenario Creation Guide](../docs/dev/scenario-creation.md)
