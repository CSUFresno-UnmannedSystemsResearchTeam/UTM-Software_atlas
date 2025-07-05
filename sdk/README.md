# ATLAS SDK - Drone Developer Kit

The ATLAS SDK enables drone manufacturers and developers to integrate their systems with the ATLAS UTM platform, allowing drones to participate in coordinated traffic management and mission operations.

## Overview

The ATLAS SDK provides:
- **C++ libraries** for drone-side integration
- **Protocol definitions** for communication with ATLAS
- **Code generation tools** for custom implementations
- **Testing framework** for validation
- **Documentation and examples** for quick integration

## SDK Components

```
sdk/
├── include/                    # SDK header files
│   ├── atlas/                 # Core SDK headers
│   │   ├── drone_interface.h  # Main drone integration interface
│   │   ├── command_handler.h  # Command processing
│   │   ├── telemetry.h        # Telemetry data structures
│   │   ├── mission.h          # Mission handling
│   │   └── communication.h    # Communication protocols
│   └── protocols/             # Protocol buffer headers (auto-generated)
├── src/                       # SDK implementation
│   ├── drone_interface.cpp    # Core interface implementation
│   ├── command_handler.cpp    # Command processing logic
│   ├── telemetry.cpp          # Telemetry handling
│   ├── mission.cpp            # Mission management
│   └── communication.cpp      # Communication handling
├── examples/                  # Example implementations
│   ├── basic_drone/           # Minimal drone implementation
│   ├── search_rescue_drone/   # Search and rescue example
│   ├── delivery_drone/        # Package delivery example
│   └── custom_payload_drone/  # Custom payload example
├── tools/                     # SDK tools and utilities
│   ├── code_generator/        # Generate boilerplate code
│   ├── validator/             # Validate implementations
│   ├── simulator/             # Testing simulator
│   └── packager/              # Package SDK distributions
├── tests/                     # SDK test suite
│   ├── unit/                  # Unit tests
│   ├── integration/           # Integration tests
│   └── mock/                  # Mock ATLAS services for testing
├── docs/                      # SDK documentation
│   ├── getting_started.md     # Quick start guide
│   ├── api_reference.md       # Complete API documentation
│   ├── integration_guide.md   # Integration best practices
│   └── examples.md            # Example explanations
├── cmake/                     # CMake modules and configuration
│   ├── FindATLAS.cmake        # CMake find module
│   └── ATLASConfig.cmake      # SDK configuration
└── packaging/                 # Distribution packaging
    ├── debian/                # Debian package configuration
    ├── rpm/                   # RPM package configuration
    └── vcpkg/                 # vcpkg port configuration
```

## Quick Start

### **Prerequisites**
- C++17 compatible compiler (GCC 7+, Clang 6+, MSVC 2019+)
- CMake 3.15 or later
- Protocol Buffers 3.12+
- gRPC 1.30+

### **Installation**

#### **Option 1: vcpkg (Recommended)**
```bash
# Add ATLAS SDK to your vcpkg.json
{
  "dependencies": ["atlas-sdk"]
}

# Install
vcpkg install
```

#### **Option 2: System Package**
```bash
# Ubuntu/Debian
sudo apt install libatlas-sdk-dev

# Build from source
git clone https://github.com/CSUFresno-UnmannedSystemsResearchTeam/UTM-Software_atlas.git
cd UTM-Software_atlas/sdk
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
```

### **Basic Integration**

#### **1. Include SDK in Your Project**
```cmake
# CMakeLists.txt
find_package(ATLAS REQUIRED)

add_executable(my_drone main.cpp)
target_link_libraries(my_drone ATLAS::SDK)
```

#### **2. Implement Drone Interface**
```cpp
// my_drone.h
#include <atlas/drone_interface.h>

class MyDrone : public atlas::DroneInterface {
public:
    // Required: Report drone capabilities
    atlas::DroneCapabilities getCapabilities() override;
    
    // Required: Execute commands from ATLAS
    atlas::CommandResponse executeCommand(const atlas::Command& command) override;
    
    // Required: Provide current telemetry
    atlas::Telemetry getCurrentTelemetry() override;
    
    // Optional: Handle mission-specific commands
    atlas::MissionResponse startMission(const atlas::Mission& mission) override;
    atlas::MissionResponse updateMission(const atlas::MissionUpdate& update) override;
    
private:
    // Your drone-specific implementation
    std::unique_ptr<MyDroneHardware> hardware_;
    atlas::TelemetryManager telemetry_manager_;
};
```

#### **3. Register Capabilities**
```cpp
// my_drone.cpp
atlas::DroneCapabilities MyDrone::getCapabilities() {
    atlas::DroneCapabilities caps;
    
    // Basic drone information
    caps.set_drone_id("my_drone_001");
    caps.set_drone_type("quadrotor");
    caps.set_manufacturer("My Company");
    caps.set_model("MC-Q400");
    caps.set_firmware_version("1.2.3");
    
    // Flight capabilities
    auto* flight_caps = caps.mutable_flight_capabilities();
    flight_caps->set_max_altitude(500);      // meters AGL
    flight_caps->set_max_speed(15.0);        // m/s
    flight_caps->set_max_flight_time(25);    // minutes
    flight_caps->set_max_range(5000);        // meters
    
    // Payload capabilities
    auto* payload_caps = caps.mutable_payload_capabilities();
    payload_caps->set_max_weight(500);       // grams
    
    // Sensor capabilities
    caps.add_sensors(atlas::SENSOR_GPS);
    caps.add_sensors(atlas::SENSOR_CAMERA_VISUAL);
    caps.add_sensors(atlas::SENSOR_IMU);
    caps.add_sensors(atlas::SENSOR_BAROMETER);
    
    // Supported commands
    caps.add_supported_commands(atlas::CMD_TAKEOFF);
    caps.add_supported_commands(atlas::CMD_LAND);
    caps.add_supported_commands(atlas::CMD_GOTO_WAYPOINT);
    caps.add_supported_commands(atlas::CMD_RETURN_TO_HOME);
    caps.add_supported_commands(atlas::CMD_START_MISSION);
    
    return caps;
}
```

#### **4. Handle Commands**
```cpp
atlas::CommandResponse MyDrone::executeCommand(const atlas::Command& command) {
    atlas::CommandResponse response;
    response.set_command_id(command.command_id());
    response.set_timestamp(atlas::getCurrentTimestamp());
    
    try {
        switch (command.type()) {
            case atlas::CMD_TAKEOFF:
                handleTakeoff(command);
                response.set_status(atlas::STATUS_SUCCESS);
                break;
                
            case atlas::CMD_LAND:
                handleLanding(command);
                response.set_status(atlas::STATUS_SUCCESS);
                break;
                
            case atlas::CMD_GOTO_WAYPOINT:
                handleGotoWaypoint(command);
                response.set_status(atlas::STATUS_SUCCESS);
                break;
                
            case atlas::CMD_START_MISSION:
                handleStartMission(command);
                response.set_status(atlas::STATUS_SUCCESS);
                break;
                
            default:
                response.set_status(atlas::STATUS_UNSUPPORTED);
                response.set_error_message("Command not supported");
        }
    } catch (const std::exception& e) {
        response.set_status(atlas::STATUS_ERROR);
        response.set_error_message(e.what());
    }
    
    return response;
}
```

#### **5. Provide Telemetry**
```cpp
atlas::Telemetry MyDrone::getCurrentTelemetry() {
    atlas::Telemetry telemetry;
    
    // Basic telemetry
    telemetry.set_drone_id("my_drone_001");
    telemetry.set_timestamp(atlas::getCurrentTimestamp());
    
    // Position and navigation
    auto current_position = hardware_->getCurrentPosition();
    auto* position = telemetry.mutable_position();
    position->set_latitude(current_position.lat);
    position->set_longitude(current_position.lon);
    position->set_altitude(current_position.alt);
    position->set_heading(current_position.heading);
    
    // Flight state
    auto* flight_state = telemetry.mutable_flight_state();
    flight_state->set_armed(hardware_->isArmed());
    flight_state->set_flying(hardware_->isFlying());
    flight_state->set_mode(hardware_->getFlightMode());
    flight_state->set_speed(hardware_->getCurrentSpeed());
    
    // System health
    auto* health = telemetry.mutable_system_health();
    health->set_battery_level(hardware_->getBatteryLevel());
    health->set_battery_voltage(hardware_->getBatteryVoltage());
    health->set_signal_strength(hardware_->getSignalStrength());
    health->set_gps_fix_type(hardware_->getGPSFixType());
    health->set_satellite_count(hardware_->getSatelliteCount());
    
    return telemetry;
}
```

#### **6. Connect to ATLAS**
```cpp
// main.cpp
#include "my_drone.h"
#include <atlas/atlas_client.h>

int main() {
    try {
        // Create your drone implementation
        auto drone = std::make_unique<MyDrone>();
        
        // Connect to ATLAS UTM system
        atlas::ATLASClient client("atlas.utm.example.com:50051");
        client.registerDrone(std::move(drone));
        
        // Start communication loop
        client.startCommunication();
        
        // Keep running until shutdown
        client.waitForShutdown();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Advanced Features

### **Mission Handling**

For drones that support autonomous missions:

```cpp
atlas::MissionResponse MyDrone::startMission(const atlas::Mission& mission) {
    atlas::MissionResponse response;
    
    if (mission.type() == atlas::MISSION_SEARCH_PATTERN) {
        // Extract search parameters
        auto search_area = mission.search_mission().search_area();
        auto pattern_type = mission.search_mission().pattern_type();
        
        // Plan flight path
        auto waypoints = planSearchPattern(search_area, pattern_type);
        
        // Upload to autopilot
        hardware_->uploadWaypoints(waypoints);
        
        // Start autonomous mission
        hardware_->startMission();
        
        response.set_status(atlas::STATUS_SUCCESS);
        response.set_estimated_duration(calculateMissionDuration(waypoints));
    } else {
        response.set_status(atlas::STATUS_UNSUPPORTED);
        response.set_error_message("Mission type not supported");
    }
    
    return response;
}
```

### **Custom Commands**

Add support for drone-specific commands:

```cpp
class SearchRescueDrone : public atlas::DroneInterface {
public:
    atlas::DroneCapabilities getCapabilities() override {
        auto caps = atlas::DroneInterface::getCapabilities();
        
        // Add custom commands
        caps.add_supported_commands(atlas::CMD_START_THERMAL_SCAN);
        caps.add_supported_commands(atlas::CMD_DEPLOY_RESCUE_EQUIPMENT);
        
        return caps;
    }
    
    atlas::CommandResponse executeCommand(const atlas::Command& command) override {
        if (command.type() == atlas::CMD_START_THERMAL_SCAN) {
            return handleThermalScan(command);
        } else if (command.type() == atlas::CMD_DEPLOY_RESCUE_EQUIPMENT) {
            return handleRescueEquipmentDeployment(command);
        }
        
        // Fall back to base implementation
        return atlas::DroneInterface::executeCommand(command);
    }
    
private:
    atlas::CommandResponse handleThermalScan(const atlas::Command& command) {
        // Enable thermal camera
        thermal_camera_->enable();
        thermal_camera_->setMode(ThermalCamera::PERSON_DETECTION);
        
        atlas::CommandResponse response;
        response.set_command_id(command.command_id());
        response.set_status(atlas::STATUS_SUCCESS);
        return response;
    }
};
```

### **Event Notifications**

Send asynchronous events to ATLAS:

```cpp
class EventAwareDrone : public atlas::DroneInterface {
public:
    void onPersonDetected(const PersonDetection& detection) {
        atlas::Event event;
        event.set_type(atlas::EVENT_PERSON_DETECTED);
        event.set_drone_id(getDroneId());
        event.set_timestamp(atlas::getCurrentTimestamp());
        
        // Add detection details
        auto* person_event = event.mutable_person_detection();
        person_event->set_confidence(detection.confidence);
        person_event->set_location_lat(detection.location.lat);
        person_event->set_location_lon(detection.location.lon);
        person_event->set_image_data(detection.image);
        
        // Send to ATLAS
        sendEvent(event);
    }
    
    void onEmergency(const EmergencyCondition& emergency) {
        atlas::Event event;
        event.set_type(atlas::EVENT_EMERGENCY);
        event.set_priority(atlas::PRIORITY_CRITICAL);
        event.set_drone_id(getDroneId());
        
        auto* emergency_event = event.mutable_emergency();
        emergency_event->set_condition_type(emergency.type);
        emergency_event->set_description(emergency.description);
        emergency_event->set_requires_immediate_landing(emergency.critical);
        
        sendEvent(event);
    }
};
```

## Testing Your Integration

### **Unit Testing**
```cpp
#include <gtest/gtest.h>
#include <atlas/testing/mock_atlas.h>
#include "my_drone.h"

class MyDroneTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_atlas_ = std::make_unique<atlas::testing::MockATLAS>();
        drone_ = std::make_unique<MyDrone>();
    }
    
    std::unique_ptr<atlas::testing::MockATLAS> mock_atlas_;
    std::unique_ptr<MyDrone> drone_;
};

TEST_F(MyDroneTest, ReportsCorrectCapabilities) {
    auto capabilities = drone_->getCapabilities();
    
    EXPECT_EQ(capabilities.drone_type(), "quadrotor");
    EXPECT_GT(capabilities.max_altitude(), 0);
    EXPECT_TRUE(capabilities.has_sensor(atlas::SENSOR_GPS));
}

TEST_F(MyDroneTest, ExecutesTakeoffCommand) {
    atlas::Command takeoff_cmd;
    takeoff_cmd.set_type(atlas::CMD_TAKEOFF);
    takeoff_cmd.set_command_id("test_takeoff_001");
    
    auto response = drone_->executeCommand(takeoff_cmd);
    
    EXPECT_EQ(response.status(), atlas::STATUS_SUCCESS);
    EXPECT_EQ(response.command_id(), "test_takeoff_001");
}
```

### **Integration Testing**
```cpp
TEST_F(MyDroneTest, IntegratesWithATLAS) {
    // Start mock ATLAS server
    mock_atlas_->start();
    
    // Connect drone
    atlas::ATLASClient client(mock_atlas_->getAddress());
    client.registerDrone(std::move(drone_));
    
    // Verify registration
    EXPECT_TRUE(mock_atlas_->isDroneRegistered("my_drone_001"));
    
    // Send test command
    auto command = atlas::Command::createTakeoffCommand();
    mock_atlas_->sendCommand("my_drone_001", command);
    
    // Verify command execution
    auto telemetry = mock_atlas_->getLatestTelemetry("my_drone_001");
    EXPECT_TRUE(telemetry.flight_state().flying());
}
```

### **Hardware-in-the-Loop Testing**
```cpp
class HILTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Connect to hardware simulator
        simulator_ = atlas::testing::DroneSimulator::create();
        simulator_->loadModel("quadrotor_x4");
        
        // Create drone with simulated hardware
        drone_ = std::make_unique<MyDrone>(simulator_->getHardwareInterface());
    }
    
    std::unique_ptr<atlas::testing::DroneSimulator> simulator_;
    std::unique_ptr<MyDrone> drone_;
};

TEST_F(HILTest, ExecutesFlightMission) {
    // Define test mission
    atlas::Mission mission;
    auto* waypoint1 = mission.add_waypoints();
    waypoint1->set_latitude(40.7128);
    waypoint1->set_longitude(-74.0060);
    waypoint1->set_altitude(100);
    
    // Execute mission
    auto response = drone_->startMission(mission);
    EXPECT_EQ(response.status(), atlas::STATUS_SUCCESS);
    
    // Simulate flight
    simulator_->run(std::chrono::minutes(5));
    
    // Verify mission completion
    auto telemetry = drone_->getCurrentTelemetry();
    EXPECT_NEAR(telemetry.position().latitude(), 40.7128, 0.0001);
    EXPECT_NEAR(telemetry.position().longitude(), -74.0060, 0.0001);
}
```

## SDK Tools

### **Code Generator**
Generate boilerplate implementation:
```bash
# Generate basic drone implementation
atlas-sdk-generate --type basic_drone --name MyDrone --output ./src/

# Generate search and rescue drone
atlas-sdk-generate --type search_rescue --name SearchDrone --output ./src/

# Generate custom implementation
atlas-sdk-generate --template custom.yaml --output ./src/
```

### **Validator**
Validate your implementation:
```bash
# Validate capabilities
atlas-sdk-validate --check capabilities ./my_drone

# Validate command handling
atlas-sdk-validate --check commands ./my_drone

# Validate telemetry
atlas-sdk-validate --check telemetry ./my_drone

# Full validation
atlas-sdk-validate --full ./my_drone
```

### **Simulator**
Test against simulated ATLAS:
```bash
# Start ATLAS simulator
atlas-sdk-simulator --start

# Run your drone against simulator
./my_drone --atlas-address localhost:50051

# Run automated test scenarios
atlas-sdk-simulator --scenario search_mission --drone ./my_drone
```

## Examples

The SDK includes several complete examples:

### **[Basic Drone](examples/basic_drone/)** 
Minimal implementation showing core concepts

### **[Search and Rescue Drone](examples/search_rescue_drone/)**
Advanced example with person detection and rescue capabilities

### **[Delivery Drone](examples/delivery_drone/)**
Package delivery with pickup and drop-off locations

### **[Custom Payload Drone](examples/custom_payload_drone/)**
Extensible example for specialized payloads

## API Reference

### **Core Classes**

#### **atlas::DroneInterface**
Base interface that all drones must implement
```cpp
class DroneInterface {
public:
    virtual ~DroneInterface() = default;
    
    // Required implementations
    virtual DroneCapabilities getCapabilities() = 0;
    virtual CommandResponse executeCommand(const Command& command) = 0;
    virtual Telemetry getCurrentTelemetry() = 0;
    
    // Optional implementations
    virtual MissionResponse startMission(const Mission& mission);
    virtual MissionResponse updateMission(const MissionUpdate& update);
    virtual void onEvent(const Event& event);
};
```

#### **atlas::ATLASClient**
Client for connecting to ATLAS UTM system
```cpp
class ATLASClient {
public:
    ATLASClient(const std::string& server_address);
    
    void registerDrone(std::unique_ptr<DroneInterface> drone);
    void startCommunication();
    void waitForShutdown();
    
    void sendEvent(const Event& event);
    void updateTelemetry(const Telemetry& telemetry);
};
```

### **Message Types**

All message types are defined in Protocol Buffers and include:
- **DroneCapabilities**: Drone specifications and supported features
- **Command**: Instructions from ATLAS to drone
- **CommandResponse**: Drone's response to commands
- **Telemetry**: Current drone state and sensor data
- **Mission**: Autonomous mission definitions
- **Event**: Asynchronous notifications from drone

For complete API documentation, see [docs/api_reference.md](docs/api_reference.md).

## Distribution and Packaging

### **Building SDK Package**
```bash
# Build development package
cd sdk
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make package

# Build distribution packages
make package_debian    # .deb package
make package_rpm       # .rpm package
make package_vcpkg     # vcpkg port
```

### **Installing from Package**
```bash
# Ubuntu/Debian
sudo dpkg -i atlas-sdk_1.0.0_amd64.deb

# Red Hat/CentOS
sudo rpm -i atlas-sdk-1.0.0.x86_64.rpm

# vcpkg
vcpkg install atlas-sdk
```

## Best Practices

### **Performance**
1. **Efficient telemetry**: Only send changed data when possible
2. **Async operations**: Don't block command execution thread
3. **Memory management**: Use RAII and smart pointers
4. **Error handling**: Graceful degradation for communication failures

### **Safety**
1. **Validation**: Validate all commands before execution
2. **Fail-safe**: Implement safe failure modes
3. **Timeouts**: Handle communication timeouts appropriately
4. **State management**: Maintain consistent internal state

### **Reliability**
1. **Logging**: Comprehensive logging for debugging
2. **Recovery**: Automatic recovery from transient failures
3. **Testing**: Thorough unit and integration testing
4. **Monitoring**: Health monitoring and self-diagnostics

### **Integration**
1. **Configuration**: Externalize configuration parameters
2. **Versioning**: Handle SDK and protocol version compatibility
3. **Documentation**: Document custom commands and capabilities
4. **Support**: Provide clear error messages and diagnostics

## Support and Community

### **Getting Help**
- **Documentation**: [Complete SDK documentation](docs/)
- **Examples**: [Working code examples](examples/)
- **GitHub Issues**: [Report bugs and request features](https://github.com/CSUFresno-UnmannedSystemsResearchTeam/UTM-Software_atlas/issues)
- **Discussions**: [Community discussions and Q&A](https://github.com/CSUFresno-UnmannedSystemsResearchTeam/UTM-Software_atlas/discussions)

### **Contributing**
We welcome contributions to the ATLAS SDK:
1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Submit a pull request

### **License**
The ATLAS SDK is released under [LICENSE](../LICENSE) terms.

---

**Ready to integrate your drone with ATLAS UTM? Start with the [Getting Started Guide](docs/getting_started.md)!**
