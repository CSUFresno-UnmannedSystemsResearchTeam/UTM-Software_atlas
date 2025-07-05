# ATLAS Simulators

This directory contains hardware and environment simulation tools for testing the ATLAS UTM system without physical drones or radio equipment.

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

## Simulator Components

### **drone_simulator/** - Virtual Drone Fleet

**Purpose**: Simulate realistic drone behavior, physics, and responses

#### **Key Features**:
- **Multiple drone types**: Quadrotor, fixed-wing, helicopter models
- **Realistic physics**: Flight dynamics, battery consumption, payload effects
- **Autonomous behaviors**: Search patterns, obstacle avoidance, return-to-home
- **Sensor simulation**: GPS, cameras, thermal sensors, obstacle detection
- **Communication simulation**: Message delays, packet loss, signal strength
- **Failure modes**: Battery depletion, mechanical failures, communication loss

#### **Drone Models** (`models/`):
```python
# Example drone model configuration
class QuadrotorModel:
    def __init__(self):
        self.max_speed = 15.0  # m/s
        self.max_altitude = 500.0  # meters
        self.battery_capacity = 5000  # mAh
        self.flight_time = 25  # minutes
        self.payload_capacity = 200  # grams
        self.sensors = ["gps", "camera", "imu", "barometer"]
        self.communication_range = 2000  # meters
```

#### **Autonomous Behaviors** (`behaviors/`):
- **Grid Search**: Systematic area coverage
- **Spiral Search**: Expanding circular search pattern
- **Random Walk**: Pseudo-random exploration
- **Waypoint Following**: Precise navigation to GPS coordinates
- **Formation Flying**: Coordinated multi-drone operations
- **Emergency Procedures**: Return-to-home, emergency landing

#### **Physics Engine** (`physics/`):
```cpp
// Example physics simulation
class DronePhysics {
    void updatePosition(double dt) {
        // Apply forces (thrust, gravity, drag, wind)
        Vector3 total_force = thrust + gravity + drag + wind_force;
        
        // Update velocity and position using physics integration
        acceleration = total_force / mass;
        velocity += acceleration * dt;
        position += velocity * dt;
        
        // Update battery based on power consumption
        battery_level -= calculatePowerConsumption(dt);
    }
};
```

### **radio_simulator/** - Communication Simulation

**Purpose**: Simulate GNU Radio, RF propagation, and communication protocols

#### **Key Features**:
- **Protocol support**: MAVLink 2, custom ATLAS protocols
- **RF propagation**: Distance-based signal degradation, interference
- **Hardware simulation**: Mock Adalm-Pluto SDR behavior
- **Network effects**: Latency, jitter, packet loss simulation
- **Frequency management**: Multiple channels, frequency conflicts

#### **Protocol Implementations** (`protocols/`):
```python
# Example MAVLink simulation
class MAVLinkSimulator:
    def __init__(self, frequency=433.0, power=1.0):
        self.frequency = frequency  # MHz
        self.transmit_power = power  # Watts
        self.packet_queue = Queue()
        
    def send_message(self, message, destination):
        # Simulate RF propagation delay
        distance = calculate_distance(self.position, destination.position)
        propagation_delay = distance / SPEED_OF_LIGHT
        
        # Calculate signal strength and packet loss
        signal_strength = self.calculate_signal_strength(distance)
        packet_loss_probability = self.calculate_packet_loss(signal_strength)
        
        if random.random() > packet_loss_probability:
            # Schedule message delivery
            schedule_delivery(message, destination, propagation_delay)
```

#### **RF Models** (`rf_models/`):
- **Free Space Path Loss**: Basic signal degradation with distance
- **Urban Propagation**: Building and obstacle effects
- **Weather Effects**: Rain fade, atmospheric ducting
- **Multipath**: Signal reflections and interference
- **Noise Models**: Thermal noise, man-made interference

### **environment_simulator/** - Environmental Conditions

**Purpose**: Simulate weather, terrain, and environmental factors

#### **Weather Simulation** (`weather/`):
```yaml
# Example weather scenario
weather_scenario:
  name: "Heavy Rain with Wind"
  conditions:
    wind:
      speed: 12.0        # m/s
      direction: 270     # degrees (west)
      gusts: true
      gust_factor: 1.5
    precipitation:
      type: "rain"
      intensity: "heavy"  # light, moderate, heavy
      visibility: 500     # meters
    temperature: 15       # Celsius
    pressure: 1013.25    # hPa
    humidity: 85         # percent
  effects:
    flight_performance:
      max_speed_reduction: 0.3
      battery_drain_increase: 1.2
      gps_accuracy_degradation: 2.0
    communication:
      signal_attenuation: 0.8
      packet_loss_increase: 0.15
```

#### **Terrain Models** (`terrain/`):
- **Digital Elevation Models**: Height maps for realistic terrain
- **Obstacle Databases**: Buildings, trees, power lines
- **No-fly Zones**: Airports, restricted airspace
- **Landing Sites**: Emergency landing location identification

#### **Interference Sources** (`interference/`):
- **RF Interference**: Cell towers, WiFi, other drones
- **GPS Jamming**: Simulated GPS denial scenarios
- **Electromagnetic**: Power lines, industrial equipment

### **scenarios/** - Pre-defined Test Scenarios

**Purpose**: Ready-to-use scenarios for testing and training

#### **Search and Rescue** (`search_rescue/`):
```yaml
# Example search and rescue scenario
scenario:
  name: "Forest Search - Missing Hiker"
  description: "Search for missing person in dense forest area"
  
  environment:
    terrain: "dense_forest"
    weather: "clear_day"
    area:
      center: [40.7128, -74.0060]
      radius: 2000  # meters
  
  mission:
    type: "search_and_rescue"
    target:
      type: "person"
      last_known_position: [40.7150, -74.0080]
      movement_pattern: "random_walk"  # lost person behavior
    
    drones:
      count: 8
      types: ["quadrotor_thermal", "quadrotor_visual"]
      deployment: "distributed_start"
    
    success_criteria:
      target_detection_required: true
      max_mission_time: 120  # minutes
      min_area_coverage: 0.85
  
  challenges:
    - tree_canopy_obstruction
    - thermal_signature_masking
    - communication_dead_zones
```

#### **Failure Modes** (`failure_modes/`):
- **Communication Loss**: Various radio failure scenarios
- **Battery Depletion**: Different battery failure patterns
- **Sensor Failures**: GPS loss, camera malfunction, IMU drift
- **Weather Events**: Sudden weather changes during missions
- **Mechanical Failures**: Motor failure, structural damage

#### **Stress Tests** (`stress_tests/`):
- **25 Drone Fleet**: Maximum simultaneous drone operations
- **High Frequency Operations**: Rapid mission cycling
- **Communication Saturation**: Maximum radio bandwidth usage
- **Database Load**: Large amounts of telemetry and log data
- **Resource Exhaustion**: Memory and CPU limit testing

#### **Training Scenarios** (`training/`):
- **Basic Operations**: Simple single-drone missions
- **Emergency Response**: Handling various emergency situations
- **Multi-operator**: Coordination between multiple operators
- **Equipment Failures**: Response to hardware malfunctions

### **integration/** - ATLAS Integration

#### **atlas_bridge.py** - Simulator Bridge
```python
class ATLASSimulatorBridge:
    """Bridge between ATLAS services and simulators"""
    
    def __init__(self):
        self.drone_sim = DroneSimulator()
        self.radio_sim = RadioSimulator()
        self.env_sim = EnvironmentSimulator()
        self.atlas_services = ATLASServiceClients()
    
    async def start_simulation(self, scenario_config):
        """Start simulation with given scenario"""
        # Configure simulators
        await self.setup_environment(scenario_config.environment)
        await self.deploy_drones(scenario_config.drones)
        
        # Connect to ATLAS services
        await self.atlas_services.connect()
        
        # Start simulation loop
        await self.run_simulation_loop()
    
    async def run_simulation_loop(self):
        """Main simulation execution loop"""
        while self.simulation_active:
            # Update simulator state
            self.drone_sim.update(self.dt)
            self.radio_sim.update(self.dt)
            self.env_sim.update(self.dt)
            
            # Exchange data with ATLAS
            await self.send_telemetry_to_atlas()
            await self.receive_commands_from_atlas()
            
            # Process any events or alerts
            await self.process_simulation_events()
            
            await asyncio.sleep(self.dt)
```

#### **scenario_runner.py** - Automated Execution
```python
class ScenarioRunner:
    """Automated scenario execution and validation"""
    
    def run_scenario(self, scenario_file):
        """Execute a scenario and validate results"""
        scenario = load_scenario(scenario_file)
        
        # Setup simulation
        bridge = ATLASSimulatorBridge()
        bridge.load_scenario(scenario)
        
        # Execute scenario
        start_time = time.time()
        results = bridge.run_until_completion()
        duration = time.time() - start_time
        
        # Validate results
        validation = self.validate_scenario_results(scenario, results)
        
        return ScenarioReport(
            scenario=scenario,
            results=results,
            duration=duration,
            validation=validation
        )
```

#### **metrics_collector.py** - Performance Monitoring
```python
class MetricsCollector:
    """Collect and analyze simulation metrics"""
    
    def __init__(self):
        self.metrics = {
            'system_performance': SystemMetrics(),
            'mission_effectiveness': MissionMetrics(),
            'communication_quality': CommMetrics(),
            'resource_usage': ResourceMetrics()
        }
    
    def collect_real_time_metrics(self):
        """Collect metrics during simulation"""
        return {
            'cpu_usage': psutil.cpu_percent(),
            'memory_usage': psutil.virtual_memory().percent,
            'network_throughput': self.measure_network_throughput(),
            'simulation_fps': self.calculate_simulation_fps(),
            'atlas_response_time': self.measure_atlas_response_time()
        }
```

### **tools/** - Simulator Tools

#### **scenario_editor.py** - Scenario Creation GUI
```python
# Qt-based scenario editor
class ScenarioEditor(QMainWindow):
    """GUI tool for creating and editing simulation scenarios"""
    
    def __init__(self):
        super().__init__()
        self.init_ui()
        self.scenario = SimulationScenario()
    
    def init_ui(self):
        # Map widget for area selection
        self.map_widget = MapWidget()
        
        # Drone configuration panel
        self.drone_panel = DroneConfigPanel()
        
        # Environment settings
        self.env_panel = EnvironmentPanel()
        
        # Mission parameters
        self.mission_panel = MissionPanel()
```

#### **visualization.py** - Real-time Visualization
```python
class SimulationVisualizer:
    """Real-time visualization of simulation state"""
    
    def __init__(self):
        self.renderer = OpenGLRenderer()
        self.map_overlay = MapOverlay()
        
    def render_frame(self, simulation_state):
        """Render current simulation state"""
        # Clear frame
        self.renderer.clear()
        
        # Render map and terrain
        self.renderer.draw_terrain(simulation_state.terrain)
        
        # Render drones
        for drone in simulation_state.drones:
            self.renderer.draw_drone(drone.position, drone.heading)
            self.renderer.draw_flight_path(drone.path_history)
        
        # Render search areas
        for area in simulation_state.search_areas:
            self.renderer.draw_search_area(area)
        
        # Render communication links
        self.renderer.draw_communication_links(simulation_state.comm_links)
        
        # Update display
        self.renderer.swap_buffers()
```

#### **analysis.py** - Post-Simulation Analysis
```python
class SimulationAnalyzer:
    """Analysis tools for completed simulations"""
    
    def analyze_mission_effectiveness(self, scenario, results):
        """Analyze how effectively the mission was completed"""
        return {
            'area_coverage': self.calculate_area_coverage(results),
            'search_efficiency': self.calculate_search_efficiency(results),
            'time_to_detection': results.detection_time,
            'resource_utilization': self.calculate_resource_usage(results),
            'operator_workload': self.analyze_operator_actions(results)
        }
    
    def generate_performance_report(self, results):
        """Generate comprehensive performance report"""
        report = PerformanceReport()
        report.add_section('Mission Summary', self.summarize_mission(results))
        report.add_section('System Performance', self.analyze_system_performance(results))
        report.add_section('Communication Analysis', self.analyze_communication(results))
        report.add_section('Recommendations', self.generate_recommendations(results))
        return report
```

## Usage Examples

### **Running Basic Simulation**
```bash
# Start simple drone simulation
./atlas simulate --scenario scenarios/search_rescue/basic_forest_search.yaml

# Run with visualization
./atlas simulate --scenario basic_search.yaml --visualize

# Run automated test suite
./atlas simulate --batch scenarios/stress_tests/
```

### **Creating Custom Scenarios**
```bash
# Open scenario editor
./atlas simulate --editor

# Create scenario from template
./atlas simulate --create-scenario --template search_rescue

# Validate scenario file
./atlas simulate --validate scenarios/my_scenario.yaml
```

### **Integration Testing**
```bash
# Test with real ATLAS services
./atlas dev --with-simulation

# Run integration tests with simulation
./atlas test e2e --simulator

# Performance testing with 25 simulated drones
./atlas test performance --drones 25 --duration 600
```

## Development

### **Adding New Drone Models**
```python
# Create new drone model
class CustomDroneModel(DroneModel):
    def __init__(self):
        super().__init__(
            name="custom_hexacopter",
            max_speed=20.0,
            flight_time=45,
            payload_capacity=500,
            sensors=["gps", "lidar", "thermal_camera"]
        )
    
    def update_physics(self, dt):
        # Implement custom flight dynamics
        pass
```

### **Creating New Scenarios**
```yaml
# Template for new scenarios
scenario:
  name: "My Custom Scenario"
  description: "Description of the scenario"
  
  environment:
    terrain: "urban"
    weather: "windy"
    obstacles: ["buildings", "power_lines"]
  
  mission:
    type: "custom_mission"
    objectives:
      - "objective_1"
      - "objective_2"
    
    drones:
      count: 3
      types: ["quadrotor"]
      start_positions: "predefined"
  
  events:
    - time: 300  # 5 minutes
      type: "weather_change"
      params:
        new_weather: "storm"
```

### **Extending Simulation Physics**
```cpp
// Add new physics effects
class WeatherPhysics : public PhysicsModule {
public:
    void applyWindForce(Drone& drone, const Wind& wind) {
        Vector3 wind_force = calculateWindForce(
            drone.getVelocity(),
            wind.getSpeed(),
            wind.getDirection(),
            drone.getDragCoefficient()
        );
        drone.addForce(wind_force);
    }
};
```

## Performance Considerations

### **Simulation Scaling**
- **Single Machine Limits**: ~25 drones with full physics simulation
- **Reduced Fidelity Mode**: 50+ drones with simplified physics
- **Distributed Simulation**: Multiple machines for larger fleets
- **Real-time vs Fast-time**: Configurable simulation speed

### **Resource Management**
```python
# Performance optimization
class SimulationOptimizer:
    def optimize_for_scale(self, drone_count):
        if drone_count <= 10:
            return HighFidelityConfig()
        elif drone_count <= 25:
            return MediumFidelityConfig()
        else:
            return LowFidelityConfig()
```

## Configuration

### **Simulator Settings**
```yaml
# simulator_config.yaml
simulation:
  timestep: 0.1           # seconds
  real_time: true         # run at real-time speed
  physics_fidelity: high  # high, medium, low
  
performance:
  max_drones: 25
  update_frequency: 10    # Hz
  memory_limit: "2GB"
  
visualization:
  enabled: true
  fps_limit: 30
  resolution: [1920, 1080]
  
logging:
  level: INFO
  telemetry_logging: true
  event_logging: true
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
