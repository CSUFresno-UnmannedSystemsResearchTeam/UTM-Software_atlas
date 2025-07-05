# ATLAS Applications and Services

This directory contains all executable applications and microservices that make up the ATLAS UTM system.

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

### **atlas/** - Main Application
- **Type**: Qt 6 QML Desktop Application
- **Purpose**: Ground Control Station interface for operators
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
├── include/            # Header files
├── src/               # Source code implementation
├── tests/             # Service-specific unit tests
├── proto/             # Protocol Buffer definitions (if applicable)
├── CMakeLists.txt     # Build configuration
└── README.md          # Service-specific documentation
```

## Build System

All applications and services use CMake with the following targets:

```bash
# Build all applications and services
cmake --build build --target apps

# Build specific application/service
cmake --build build --target atlas
cmake --build build --target protocol-service

# Run tests for all apps
cmake --build build --target test-apps

# Run tests for specific app
cmake --build build --target test-atlas
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

## Dependencies

### Common Dependencies (All Apps)
- C++20 compatible compiler
- CMake 3.25+
- Qt 6.5+ (for atlas application)
- gRPC and Protocol Buffers
- Boost libraries
- atlas-common library

### External Dependencies
- PostgreSQL client library
- Redis client library
- OpenCV (for video processing)
- GNU Radio (for radio service)

## Testing

Each application/service includes:
- **Unit tests**: Component-specific testing
- **Integration tests**: Service interaction testing (in ../tests/integration/)
- **End-to-end tests**: Full system testing (in ../tests/e2e/)

Run all app tests:
```bash
# From repository root
./atlas test apps
```

## Deployment

### Development
Services run locally during development:
```bash
# Start all services
./atlas dev

# Start specific service
./atlas dev --service protocol
```

### Production
Services are containerized and deployed via Docker Compose or Kubernetes:
```bash
# Docker deployment
docker-compose -f docker/prod/docker-compose.yml up

# Kubernetes deployment
kubectl apply -f deploy/k8s/
```

## Service Descriptions

### **protocol/**
Handles all drone communication protocols (MAVLink, custom protocols).

### **mission/**
Plans and coordinates multi-drone missions, path planning, area distribution.

### **geofencing/**
Manages airspace boundaries, violation detection, traffic management.

### **telemetry/**
Processes real-time drone data, state management, data streaming.

### **alert/**
Manages notifications, alerts, operator confirmations.

### **radio/**
Interfaces with GNU Radio hardware, handles RF communication.

### **database/**
Provides data persistence services, caching, transaction management.

## Getting Started

1. **Set up development environment**:
   ```bash
   ./atlas setup
   ```

2. **Build all applications**:
   ```bash
   ./atlas build
   ```

3. **Run tests**:
   ```bash
   ./atlas test apps
   ```

4. **Start development services**:
   ```bash
   ./atlas dev
   ```

5. **Start the main application**:
   ```bash
   ./atlas dev --service atlas
   ```

For detailed information about each application/service, see their individual README.md files.
