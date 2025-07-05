# ATLAS Applications and Services

This directory contains all executable applications and microservices that make up the ATLAS system.

## Structure

```
apps/
├── atlas/              # Main Qt QML application (Ground Control Station)
├── protocol/           # Protocol handling and translation service
├── mission/            # Mission planning and coordination service
├── geofencing/         # Airspace management service
├── telemetry/          # Real-time data processing service
├── alert/              # Notification management service
├── radio/              # Hardware communication interface service
└── database/           # Data persistence service
```

## Application Types

### **atlas** - Main Application
- **Type**: Qt 6 QML Desktop Application
- **Purpose**: Unmanned traffic management (UTM) system and mission planner 
- **Technology**: C++20, Qt 6, QML
- **Responsibilities**:
  - Real-time drone monitoring and control
  - Mission planning interface
  - Video feed display
  - Alert management
  - User interface for all system functions

### **Services** - Microservices
- **Type**: gRPC-based microservices
- **Purpose**: Backend processing and business logic
- **Technology**: C++20, gRPC, Protocol Buffers
- **Communication**: Service-to-service via gRPC, external via REST/WebSockets

## Service Architecture

Each service follows the same structure:
```
service-name/
├── include/           # Header files
├── src/               # Source code implementation
├── tests/             # Service-specific unit tests
├── CMakeLists.txt     # Build configuration
└── README.md          # Service-specific documentation
```

## Development Workflow

### Adding a New Service

1. **Create directory structure**:
   ```bash
   mkdir apps/new-service
   mkdir apps/new-service/{include,src,tests,proto}
   ```

2. **Create CMakeLists.txt**:
   ```cmake
   add_executable(new-service)
   target_sources(new-service PRIVATE src/main.cpp)
   target_link_libraries(new-service PRIVATE atlas-common)
   ```

3. **Define service interface** (in proto/ if gRPC service)

4. **Implement service logic** in src/

5. **Add tests** in tests/

6. **Update parent CMakeLists.txt** to include new service

### Service Communication

Services communicate using:
- **gRPC**: For service-to-service communication
- **Protocol Buffers**: For message serialization
- **REST/WebSockets**: For external client communication

Example service discovery:
```cpp
// Services register themselves with a service registry
ServiceRegistry::getInstance().registerService("mission", "localhost:50052");

// Other services can discover and connect
auto mission_client = ServiceRegistry::getInstance().getClient<MissionService>("mission");
```
