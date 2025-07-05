# ATLAS Testing Framework

This directory contains system-wide testing infrastructure for the ATLAS UTM project, complementing the component-specific tests located in each application and service directory.

## Overview

ATLAS uses a comprehensive testing strategy with multiple test levels:
- **Unit tests**: Component-specific tests (in `apps/*/tests/`)
- **Integration tests**: Service-to-service interaction tests
- **End-to-end tests**: Complete system workflow tests
- **Performance tests**: Load testing and benchmarking

## Structure

```
tests/
├── unit/                   # Cross-component unit tests
│   ├── libs/              # Tests for shared libraries
│   ├── common/            # Tests for common utilities
│   └── fixtures/          # Shared test data and fixtures
├── integration/           # Service integration tests
│   ├── service_communication/ # gRPC service interaction tests
│   ├── database/          # Database integration tests
│   ├── external_apis/     # External service integration tests
│   └── protocol/          # Protocol compatibility tests
├── e2e/                   # End-to-end system tests
│   ├── mission_workflows/ # Complete mission execution tests
│   ├── user_scenarios/    # User interaction scenarios
│   ├── failure_scenarios/ # Error handling and recovery tests
│   └── performance/       # System-wide performance tests
└── fixtures/              # Global test data and utilities
    ├── test_data/         # Sample data files
    ├── mock_services/     # Mock service implementations
    ├── test_databases/    # Test database schemas and data
    └── utilities/         # Test utilities and helpers
```

## Test Categories

### **unit/** - Cross-Component Unit Tests

**Purpose**: Test shared libraries and utilities that are used across multiple components

**What's tested here**:
- Shared library functions (`libs/common/`)
- Utility classes and helpers
- Protocol buffer message handling
- Configuration parsing and validation
- Mathematical algorithms (coordinate transformations, path planning)

**Example tests**:
```cpp
// tests/unit/libs/test_coordinate_utils.cpp
TEST(CoordinateUtils, ConvertLatLonToUTM) {
    auto utm = CoordinateUtils::latLonToUTM(40.7128, -74.0060);
    EXPECT_NEAR(utm.easting, 583960, 1);
    EXPECT_NEAR(utm.northing, 4507523, 1);
}
```

**Not tested here**: Application-specific logic (that goes in `apps/*/tests/`)

### **integration/** - Service Integration Tests

**Purpose**: Test interactions between different services and external systems

#### **service_communication/**
Tests gRPC communication between ATLAS services:
```cpp
// Example: Protocol service → Mission service communication
TEST(ServiceIntegration, ProtocolToMissionCommunication) {
    // Start both services
    auto protocol_service = startProtocolService();
    auto mission_service = startMissionService();
    
    // Send command through protocol service
    auto response = protocol_service->forwardCommand(mission_command);
    
    // Verify mission service received and processed command
    EXPECT_EQ(response.status(), SUCCESS);
    EXPECT_TRUE(mission_service->hasActiveCommand());
}
```

#### **database/**
Tests database operations and data persistence:
```cpp
// Example: Database integration test
TEST(DatabaseIntegration, MissionDataPersistence) {
    // Create mission through service
    auto mission = createTestMission();
    mission_service->saveMission(mission);
    
    // Verify data persisted correctly
    auto retrieved = mission_service->getMission(mission.id());
    EXPECT_EQ(retrieved.drone_count(), mission.drone_count());
}
```

#### **external_apis/**
Tests integration with external systems:
- Mock drone communication
- Weather service APIs
- Map tile services
- Authentication systems

#### **protocol/**
Tests protocol compatibility and message handling:
- MAVLink message parsing
- Protocol buffer serialization/deserialization
- Version compatibility testing
- Error handling for malformed messages

### **e2e/** - End-to-End System Tests

**Purpose**: Test complete user workflows and system behavior

#### **mission_workflows/**
Tests complete mission execution from start to finish:
```python
# Example: Search and rescue mission workflow
def test_complete_search_mission():
    # 1. User creates mission through UI
    mission = create_search_mission(
        area=SearchArea(lat=40.7, lon=-74.0, radius=1000),
        target_image="missing_person.jpg",
        drone_count=5
    )
    
    # 2. System distributes mission to drones
    assert mission.status == "PLANNING"
    start_mission(mission.id)
    
    # 3. Drones execute search pattern
    wait_for_drones_to_start(mission.id)
    assert all_drones_active(mission.id)
    
    # 4. Drone detects target
    simulate_target_detection(drone_id="drone1", confidence=0.95)
    
    # 5. Operator confirms detection
    alert = wait_for_alert(mission.id)
    confirm_target_detection(alert.id)
    
    # 6. Mission completes successfully
    assert mission.status == "COMPLETED"
    assert mission.target_found == True
```

#### **user_scenarios/**
Tests user interaction patterns:
- New operator onboarding workflow
- Emergency response procedures
- Multi-operator coordination
- System configuration changes

#### **failure_scenarios/**
Tests error handling and system resilience:
- Service failure recovery
- Database connection loss
- Drone communication loss
- Network partition handling
- Resource exhaustion scenarios

#### **performance/**
Tests system performance under load:
- 25 drone simultaneous operation
- High-frequency telemetry processing
- Video streaming under load
- Database performance with large datasets

### **fixtures/** - Test Data and Utilities

**Purpose**: Shared test data, mock services, and testing utilities

#### **test_data/**
Sample data files for testing:
```
test_data/
├── missions/
│   ├── search_mission_simple.json
│   ├── search_mission_complex.json
│   └── patrol_mission.json
├── telemetry/
│   ├── drone_telemetry_sample.json
│   └── high_frequency_telemetry.json
├── maps/
│   ├── test_area_small.geojson
│   └── test_area_large.geojson
└── images/
    ├── target_photos/
    └── detection_samples/
```

#### **mock_services/**
Mock implementations for testing:
```python
# Example mock drone service
class MockDroneService:
    def __init__(self):
        self.drones = {}
        self.telemetry_interval = 5.0
    
    def register_drone(self, drone_id, capabilities):
        self.drones[drone_id] = MockDrone(drone_id, capabilities)
    
    def send_command(self, drone_id, command):
        return self.drones[drone_id].execute_command(command)
    
    def get_telemetry(self, drone_id):
        return self.drones[drone_id].get_current_telemetry()
```

#### **test_databases/**
Test database configurations:
- Schema definitions for test databases
- Seed data for various test scenarios
- Migration scripts for test environments

#### **utilities/**
Testing helper functions:
```python
# Example test utilities
def wait_for_condition(condition_func, timeout=30):
    """Wait for a condition to become true with timeout."""
    
def create_test_mission(**kwargs):
    """Create a mission with sensible defaults for testing."""
    
def simulate_drone_telemetry(drone_id, duration=60):
    """Simulate realistic drone telemetry data."""
    
def assert_mission_state(mission_id, expected_state):
    """Assert mission is in expected state with detailed error info."""
```

## Test Execution

### **Running Tests**

#### **All tests**:
```bash
# Run all system tests
./atlas test

# Run with coverage
./atlas test --coverage

# Run in parallel for speed
./atlas test --parallel
```

#### **Specific test categories**:
```bash
# Unit tests only
./atlas test unit

# Integration tests only
./atlas test integration

# End-to-end tests only
./atlas test e2e

# Performance tests only
./atlas test performance
```

#### **Specific test suites**:
```bash
# Test specific integration suite
./atlas test integration/service_communication

# Test specific e2e workflow
./atlas test e2e/mission_workflows

# Test with specific tags
./atlas test --tag search_rescue
./atlas test --tag performance
```

### **Test Environment Setup**

#### **Docker-based testing** (Recommended):
```bash
# Start test environment
docker-compose -f docker/ci/test-compose.yml up -d

# Run tests in container
docker exec -it atlas-ci ./atlas test

# Clean up after tests
docker-compose -f docker/ci/test-compose.yml down -v
```

#### **Local testing**:
```bash
# Setup test databases
./atlas db migrate --env test
./atlas db seed --env test

# Start mock services
./atlas test --setup-mocks

# Run tests
./atlas test

# Cleanup
./atlas test --cleanup
```

## Test Configuration

### **Environment Variables**
```bash
# Test environment configuration
export ATLAS_ENV=testing
export ATLAS_LOG_LEVEL=DEBUG
export ATLAS_TEST_DATABASE_URL=postgresql://atlas:atlas@localhost:5432/atlas_test
export ATLAS_TEST_REDIS_URL=redis://localhost:6379/1
export ATLAS_TEST_TIMEOUT=300
export ATLAS_TEST_PARALLEL=true
```

### **Test Configuration Files**
```yaml
# tests/config/test_config.yaml
test_environment:
  database:
    url: postgresql://atlas:atlas@localhost:5432/atlas_test
    pool_size: 10
  redis:
    url: redis://localhost:6379/1
  services:
    startup_timeout: 30
    health_check_interval: 5
  performance:
    max_test_duration: 600
    resource_limits:
      memory: "1GB"
      cpu: "2.0"
```

## Test Data Management

### **Test Database**
- Separate test database: `atlas_test`
- Automatic schema migration before tests
- Data cleanup between test runs
- Isolated transactions for concurrent testing

### **Mock Data Generation**
```python
# Example: Generate realistic test data
def generate_drone_fleet(count=5):
    """Generate a fleet of mock drones with realistic capabilities."""
    return [
        MockDrone(
            id=f"drone_{i}",
            type=random.choice(["quadrotor", "fixed_wing"]),
            capabilities=["camera", "thermal", "gps"],
            battery_level=random.uniform(0.8, 1.0)
        )
        for i in range(count)
    ]
```

### **Data Isolation**
- Each test gets fresh database state
- Mock services reset between tests
- File system isolation for test artifacts
- Network isolation for integration tests

## Performance Testing

### **Load Testing**
```python
# Example: Load test with 25 drones
@pytest.mark.performance
def test_25_drone_load():
    # Create 25 mock drones
    drones = generate_drone_fleet(25)
    
    # Start telemetry streams
    for drone in drones:
        start_telemetry_stream(drone.id, frequency=1.0)  # 1Hz
    
    # Measure system performance
    start_time = time.time()
    
    # Run for 5 minutes
    time.sleep(300)
    
    # Verify system remained responsive
    response_time = measure_ui_response_time()
    assert response_time < 1.0  # UI must respond within 1 second
    
    # Verify memory usage stayed within limits
    memory_usage = get_system_memory_usage()
    assert memory_usage < 400 * 1024 * 1024  # 400MB limit
```

### **Performance Benchmarks**
- UI responsiveness under load
- Database query performance
- Service response times
- Memory usage patterns
- Network bandwidth utilization

## Continuous Integration

### **CI Pipeline Integration**
```yaml
# Example GitHub Actions integration
- name: Run ATLAS Tests
  run: |
    ./atlas test unit
    ./atlas test integration
    ./atlas test e2e --tag smoke
  env:
    ATLAS_ENV: ci
    DATABASE_URL: ${{ secrets.TEST_DATABASE_URL }}

- name: Performance Tests
  run: ./atlas test performance
  if: github.event_name == 'schedule'  # Nightly only
```

### **Test Reports**
- JUnit XML format for CI integration
- HTML coverage reports
- Performance benchmark reports
- Test failure analysis and logs

## Best Practices

### **Writing Good Tests**
1. **Descriptive test names**: `test_mission_completes_when_all_drones_finish_search`
2. **Arrange-Act-Assert pattern**: Clear test structure
3. **Independent tests**: No dependencies between tests
4. **Realistic test data**: Use data that resembles production
5. **Proper cleanup**: Always clean up resources

### **Integration Test Guidelines**
1. **Test real interactions**: Don't mock everything
2. **Use test doubles appropriately**: Mock external dependencies
3. **Verify end-to-end behavior**: Test complete workflows
4. **Handle timing issues**: Use proper waits and timeouts
5. **Test error conditions**: Verify error handling works

### **Performance Test Guidelines**
1. **Establish baselines**: Know what "good" performance looks like
2. **Test realistic scenarios**: Use production-like data volumes
3. **Monitor resource usage**: CPU, memory, network, disk
4. **Test scalability limits**: Find breaking points
5. **Automate performance regression detection**: Fail builds on performance regressions

For more detailed testing information, see:
- [Testing Strategy Guide](../docs/dev/testing-strategy.md)
- [Writing Integration Tests](../docs/dev/integration-testing.md)
- [Performance Testing Guide](../docs/dev/performance-testing.md)
