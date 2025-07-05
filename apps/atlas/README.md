# ATLAS Ground Control Station

The main Qt QML application providing the operator interface for the ATLAS UTM system.

## Overview

ATLAS is the primary Ground Control Station (GCS) application that operators use to:
- Monitor and control up to 25 autonomous drones simultaneously
- Plan and execute search and rescue missions
- Manage geofencing and airspace restrictions
- Process alerts and confirmations from drones
- Display live video feeds and telemetry data

## Architecture

### Technology Stack
- **Framework**: Qt 6 with QML/QtQuick
- **Language**: C++20
- **UI Pattern**: Model-View-Controller (MVC)
- **Styling**: Dark tactical theme (Anduril-inspired)
- **Communication**: gRPC clients to backend services

### Project Structure
```
atlas/
├── include/
│   ├── controllers/        # MVC Controllers
│   ├── models/            # Data models and business logic
│   ├── services/          # Service client wrappers
│   └── utils/             # Utility classes
├── src/
│   ├── controllers/       # Controller implementations
│   ├── models/           # Model implementations
│   ├── services/         # Service client implementations
│   ├── utils/            # Utility implementations
│   └── main.cpp          # Application entry point
├── qml/
│   ├── views/            # Main application views
│   ├── components/       # Reusable UI components
│   ├── dialogs/          # Modal dialogs
│   └── main.qml          # Root QML file
├── resources/
│   ├── images/           # Images and icons
│   ├── maps/             # Offline map data
│   ├── fonts/            # Custom fonts
│   └── qml.qrc           # Qt resource file
├── tests/
│   ├── unit/             # Unit tests
│   ├── integration/      # Integration tests with services
│   └── ui/               # QML UI tests
├── CMakeLists.txt        # Build configuration
└── README.md
```

## User Interface Design

### Application Layout
```
┌─────────────────────────────────────────────────────────┐
│ ┌─────────────┐ ┌───────────────────────────────────────┤
│ │    LOGO     │ │                                       │
│ ├─────────────┤ │                                       │
│ │ ► Home      │ │            Main Content Area          │
│ │   Drones    │ │                                       │
│ │   Monitor   │ │      (Displays selected section)     │
│ │   Logs      │ │                                       │
│ │   Operation │ │                                       │
│ │   Radio     │ │                                       │
│ │             │ │                                       │
│ │ [🔔 Alerts] │ │                                       │
│ │ Status Bar  │ │                                       │
│ └─────────────┘ └───────────────────────────────────────┤
└─────────────────────────────────────────────────────────┘
```

### Main Views

#### **Home Page** - Mission Command Center
- Overall mission status and system health dashboard
- Active operations and progress tracking
- Critical alerts requiring immediate attention
- System performance metrics
- Quick action buttons

#### **Drones Page** - Fleet Management
- Register and configure drone fleet
- Organize drones into groups
- Test communication links
- Manage drone-specific settings
- Visual drone identification

#### **Monitor Page** - Real-Time Operations
```
┌─────────────────┬─────────────────────────────────────┐
│                 │  Selected Drone Information         │
│                 │  ├── ID, Status, Battery           │
│                 │  ├── GPS Coordinates               │
│     MAP         │  ├── Current Speed, Altitude       │
│   (Left 60%)    │  ├── Mission Progress              │
│                 │  └── Control Buttons               │
│  • Drone icons  ├─────────────────────────────────────┤
│  • Flight paths │                                     │
│  • Search areas │     Video Feed Display Window       │
│  • Boundaries   │       (720p max resolution)        │
│                 │                                     │
└─────────────────┴─────────────────────────────────────┘
```

#### **Logs Page** - Mission Analysis
- Mission history and flight data
- Search and filter capabilities
- Data export functionality
- Performance analytics

#### **Operation Page** - Mission Planning
- Interactive mission planning interface
- Search area definition and distribution
- Target information management
- Mission parameter configuration

#### **Radio Page** - Communication Management
- Radio frequency and power configuration
- Communication quality monitoring
- Protocol settings and debugging

## MVC Architecture

### Models (`include/models/`, `src/models/`)
- **DroneModel**: Individual drone state and properties
- **FleetModel**: Collection of drones and fleet operations
- **MissionModel**: Mission planning and execution state
- **MapModel**: Geographic data and visualization
- **AlertModel**: Alert queue and notification management
- **VideoModel**: Video stream management and display

### Views (`qml/views/`)
- **HomeView.qml**: Dashboard and system overview
- **DronesView.qml**: Fleet management interface
- **MonitorView.qml**: Real-time operations display
- **LogsView.qml**: Historical data and analysis
- **OperationView.qml**: Mission planning interface
- **RadioView.qml**: Communication configuration

### Controllers (`include/controllers/`, `src/controllers/`)
- **MainController**: Application lifecycle and navigation
- **DroneController**: Drone management and communication
- **MissionController**: Mission planning and execution
- **MapController**: Map display and interaction
- **AlertController**: Alert processing and display
- **VideoController**: Video stream management

## Key Features

### Real-Time Monitoring
- Live drone position tracking on offline maps
- Telemetry data display (battery, altitude, speed, heading)
- Flight path visualization
- Search area coverage tracking

### Mission Planning
- Interactive map-based area selection
- Automatic GPS coordinate distribution to drones
- Target photo management for search operations
- Safety parameter configuration

### Alert Management
- Two-tier alert system:
  - **Critical alerts**: Modal dialogs requiring immediate response
  - **System alerts**: Toast notifications for informational updates
- Person detection confirmations with 720p snapshots
- Alert queue and history tracking

### Video Processing
- Live video feed display from drone cameras
- Adaptive quality based on radio bandwidth
- Multiple camera switching
- Video recording and snapshot capabilities

## Build Configuration

### Dependencies
```cmake
# Required Qt modules
find_package(Qt6 REQUIRED COMPONENTS 
    Core Widgets Quick QuickControls2 
    Multimedia Location Positioning)

# Service communication
find_package(gRPC REQUIRED)
find_package(Protobuf REQUIRED)

# Additional libraries
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED)
```

### Build Targets
```bash
# Build atlas application
cmake --build build --target atlas

# Run atlas unit tests
cmake --build build --target test-atlas

# Install atlas (development)
cmake --build build --target install-atlas
```

## Testing

### Unit Tests (`tests/unit/`)
- Model logic testing
- Controller functionality
- Service client integration
- Utility function validation

### Integration Tests (`tests/integration/`)
- Service communication testing
- Database integration
- External API integration

### UI Tests (`tests/ui/`)
- QML component testing
- User interaction simulation
- Visual regression testing

### Running Tests
```bash
# All atlas tests
./atlas test --service atlas

# Specific test categories
./atlas test --service atlas --category unit
./atlas test --service atlas --category integration
./atlas test --service atlas --category ui

# Test coverage
./atlas test --service atlas --coverage
```

## Configuration

### Application Settings
- Stored in QSettings (platform-specific)
- Runtime configuration via environment variables
- Service endpoint configuration
- User preferences and window layouts

### Environment Variables
```bash
# Service endpoints
export ATLAS_PROTOCOL_SERVICE=localhost:50051
export ATLAS_MISSION_SERVICE=localhost:50052
export ATLAS_DATABASE_SERVICE=localhost:50057

# Application settings
export ATLAS_LOG_LEVEL=DEBUG
export ATLAS_MAP_CACHE_DIR=/tmp/atlas-maps
export ATLAS_VIDEO_QUALITY=HIGH
```

## Development

### Running Locally
```bash
# Start backend services first
./atlas dev --services-only

# Start atlas application
./atlas dev --service atlas

# Or run directly
cd build/apps/atlas
./atlas
```

### QML Development
- Use Qt Creator for QML editing and live preview
- QML files are hot-reloadable during development
- Use `qmlls` language server for code completion

### Adding New Views
1. Create QML file in `qml/views/`
2. Add navigation entry in main sidebar
3. Create corresponding controller if needed
4. Add route handling in MainController

### Adding New Models
1. Create header in `include/models/`
2. Implement in `src/models/`
3. Register with QML engine in main.cpp
4. Add unit tests in `tests/unit/models/`

## Performance Considerations

### Memory Management
- 400MB total budget for 25 drones
- ~12MB per drone (telemetry, video buffers, metadata)
- Efficient QML object lifecycle management
- Smart pointer usage in C++ code

### Real-Time Requirements
- 5-second telemetry update cycle
- <1 second command response time
- 20-30 FPS video display capability
- Responsive UI during high data throughput

## Deployment

### Development
```bash
# Start with development services
./atlas dev
```

### Production
```bash
# Containerized deployment
docker run -d --name atlas-app \
  -p 8080:8080 \
  -e ATLAS_ENV=production \
  ghcr.io/csufresno-unmannedsystemsresearchteam/atlas:latest
```

## Troubleshooting

### Common Issues

**Application won't start:**
- Check service dependencies are running
- Verify Qt libraries are installed
- Check log output for specific errors

**UI performance issues:**
- Monitor memory usage with system tools
- Check for QML binding loops
- Verify video codec acceleration

**Service connection failures:**
- Verify service endpoints in configuration
- Check network connectivity
- Review service health status

### Debug Mode
```bash
# Run with debug logging
ATLAS_LOG_LEVEL=DEBUG ./atlas

# Run with QML debugging
QML_IMPORT_TRACE=1 ./atlas

# Run with graphics debugging
QSG_INFO=1 ./atlas
```

For more information, see the [ATLAS Development Guide](../../docs/dev/atlas-development.md).
