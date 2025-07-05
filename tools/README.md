# ATLAS Development Tools

This directory contains all development tools, utilities, and automation scripts for the ATLAS UTM project.

## Overview

The tools directory provides a comprehensive set of Python-based utilities accessed through the central `atlas` command. These tools streamline development, testing, deployment, and maintenance of the ATLAS system.

## Structure

```
tools/
├── atlas                   # Central command-line tool (Python script)
├── atlas_cli/             # Core CLI implementation
│   ├── __init__.py
│   ├── commands/          # Individual command implementations
│   ├── utils/             # Shared utilities
│   └── config.py          # Configuration management
├── generators/            # Code generation tools
│   ├── proto_generator.py # Protocol Buffer code generation
│   ├── doc_generator.py   # Documentation generation
│   ├── sdk_generator.py   # SDK generation for drone developers
│   └── cmake_generator.py # CMake file generation
├── validators/            # Validation and linting tools
│   ├── proto_validator.py # Protocol Buffer validation
│   ├── config_validator.py # Configuration validation
│   ├── code_formatter.py  # Code formatting (clang-format wrapper)
│   └── dependency_checker.py # Dependency validation
├── deployment/            # Deployment automation
│   ├── docker_manager.py  # Docker container management
│   ├── k8s_deployer.py    # Kubernetes deployment
│   ├── db_migrator.py     # Database migration management
│   └── service_health.py  # Service health monitoring
├── testing/               # Testing utilities
│   ├── test_runner.py     # Unified test execution
│   ├── coverage_reporter.py # Code coverage analysis
│   ├── performance_tester.py # Performance benchmarking
│   └── simulator_controller.py # Hardware simulator control
└── build/                 # Build system utilities
    ├── cmake_wrapper.py   # CMake build management
    ├── vcpkg_manager.py   # vcpkg package management
    ├── dependency_resolver.py # Dependency resolution
    └── artifact_manager.py # Build artifact management
```

## Central Atlas Command

The `atlas` script is the primary interface for all development operations:

```bash
# General syntax
./atlas <command> [subcommand] [options] [arguments]
```

### Core Commands

#### **Setup and Environment**
```bash
atlas setup                     # Initial environment setup
atlas setup --production        # Production environment setup
atlas setup --codespace         # GitHub Codespace setup

atlas env check                 # Check environment health
atlas env info                  # Display environment information
atlas env clean                 # Clean temporary files
```

#### **Build System**
```bash
atlas build                     # Build all components
atlas build --service protocol  # Build specific service
atlas build --release           # Release build
atlas build --clean             # Clean build

atlas deps install              # Install dependencies
atlas deps update               # Update dependencies
atlas deps list                 # List installed dependencies
```

#### **Development Server**
```bash
atlas dev                       # Start all development services
atlas dev --service atlas       # Start specific service
atlas dev --mock                # Start with mock services
atlas dev --debug               # Start with debugging enabled

atlas status                    # Check service health
atlas logs                      # View all service logs
atlas logs --service mission    # View specific service logs
```

#### **Testing**
```bash
atlas test                      # Run all tests
atlas test unit                 # Run unit tests only
atlas test integration          # Run integration tests
atlas test e2e                  # Run end-to-end tests
atlas test --service protocol   # Test specific service
atlas test --coverage           # Generate coverage report
```

#### **Code Generation**
```bash
atlas generate proto            # Generate Protocol Buffer code
atlas generate docs             # Generate documentation
atlas generate sdk              # Generate SDK for drone developers
atlas generate cmake            # Generate CMake files
```

#### **Database Management**
```bash
atlas db migrate                # Run database migrations
atlas db migrate --rollback     # Rollback last migration
atlas db seed                   # Seed database with test data
atlas db reset                  # Reset database to clean state
atlas db backup                 # Create database backup
atlas db restore <backup>       # Restore from backup
```

#### **Deployment**
```bash
atlas deploy local              # Deploy locally
atlas deploy staging            # Deploy to staging environment
atlas deploy production         # Deploy to production
atlas deploy --service radio    # Deploy specific service

atlas docker build              # Build Docker images
atlas docker push               # Push images to registry
atlas docker clean              # Clean Docker artifacts
```

#### **Utilities**
```bash
atlas validate                  # Validate all configurations
atlas validate proto            # Validate Protocol Buffer files
atlas validate config           # Validate configuration files

atlas format                    # Format all code
atlas format --check            # Check code formatting
atlas lint                      # Run all linters

atlas clean                     # Clean all build artifacts
atlas clean --deep              # Deep clean including dependencies
```

## Tool Implementations

### **generators/** - Code Generation

#### **proto_generator.py**
- Generates C++ and Python code from Protocol Buffer definitions
- Creates service stubs and client libraries
- Handles dependency resolution between proto files
- Validates proto syntax and compatibility

```bash
atlas generate proto
atlas generate proto --language cpp
atlas generate proto --service mission
```

#### **doc_generator.py**
- Generates API documentation from code annotations
- Creates user manuals from markdown sources
- Builds architecture diagrams from PlantUML
- Generates SDK documentation

```bash
atlas generate docs
atlas generate docs --api-only
atlas generate docs --format html
```

#### **sdk_generator.py**
- Creates drone developer SDK packages
- Generates example implementations
- Creates SDK documentation and tutorials
- Packages SDK for distribution

```bash
atlas generate sdk
atlas generate sdk --language cpp
atlas generate sdk --examples
```

### **validators/** - Code Quality

#### **proto_validator.py**
- Validates Protocol Buffer syntax and semantics
- Checks API compatibility between versions
- Ensures consistent naming conventions
- Validates message dependencies

#### **config_validator.py**
- Validates YAML/JSON configuration files
- Checks environment-specific configurations
- Validates service discovery settings
- Ensures security configuration compliance

#### **code_formatter.py**
- Wraps clang-format for C++ code formatting
- Formats Python code using black
- Formats QML files
- Checks formatting compliance in CI

### **deployment/** - Deployment Automation

#### **docker_manager.py**
- Builds Docker images for all services
- Manages multi-stage builds
- Handles image tagging and versioning
- Pushes to container registries

#### **k8s_deployer.py**
- Deploys services to Kubernetes clusters
- Manages rolling updates and rollbacks
- Handles secrets and configuration management
- Monitors deployment health

#### **db_migrator.py**
- Manages database schema migrations
- Handles data migrations and transformations
- Supports rollback and recovery
- Validates migration integrity

### **testing/** - Test Automation

#### **test_runner.py**
- Unified test execution across all test types
- Parallel test execution for performance
- Test result aggregation and reporting
- Integration with CI/CD systems

#### **coverage_reporter.py**
- Collects code coverage from all test runs
- Generates HTML and XML coverage reports
- Tracks coverage trends over time
- Enforces minimum coverage thresholds

#### **performance_tester.py**
- Runs performance benchmarks
- Load testing with simulated drone fleets
- Memory usage profiling
- Latency and throughput measurement

### **build/** - Build System Management

#### **cmake_wrapper.py**
- Simplifies CMake configuration and building
- Manages build configurations (Debug/Release)
- Handles cross-compilation settings
- Integrates with vcpkg for dependencies

#### **vcpkg_manager.py**
- Manages C++ package dependencies
- Handles vcpkg installation and updates
- Resolves dependency conflicts
- Caches packages for faster builds

## Configuration

### **atlas_cli/config.py**
Central configuration management for all tools:

```python
# Example configuration structure
ATLAS_CONFIG = {
    'build': {
        'cmake_generator': 'Ninja',
        'default_build_type': 'Debug',
        'parallel_jobs': 8
    },
    'services': {
        'protocol': {'port': 50051},
        'mission': {'port': 50052},
        # ... other services
    },
    'database': {
        'dev_url': 'postgresql://atlas:atlas@localhost:5432/atlas_dev',
        'test_url': 'postgresql://atlas:atlas@localhost:5432/atlas_test'
    },
    'docker': {
        'registry': 'ghcr.io/csufresno-unmannedsystemsresearchteam',
        'build_context': '.'
    }
}
```

### Environment Variables
```bash
# Override configuration with environment variables
export ATLAS_BUILD_TYPE=Release
export ATLAS_LOG_LEVEL=DEBUG
export ATLAS_DATABASE_URL=postgresql://...
export ATLAS_DOCKER_REGISTRY=my-registry.com
```

## Development

### Adding New Commands

1. **Create command module**:
   ```python
   # atlas_cli/commands/new_command.py
   import click
   
   @click.command()
   @click.option('--option', help='Command option')
   def new_command(option):
       """Description of new command."""
       # Implementation
   ```

2. **Register command**:
   ```python
   # atlas_cli/__init__.py
   from .commands.new_command import new_command
   
   cli.add_command(new_command)
   ```

3. **Add tests**:
   ```python
   # tests/unit/tools/test_new_command.py
   def test_new_command():
       # Test implementation
   ```

### Command Structure
All commands follow consistent patterns:
- Use Click for command-line interface
- Include comprehensive help text
- Support both interactive and CI modes
- Provide progress indicators for long operations
- Handle errors gracefully with meaningful messages

### Logging and Output
- Use structured logging with configurable levels
- Provide progress bars for long operations
- Support both human-readable and machine-readable output
- Include timing information for performance analysis

## Dependencies

### Python Requirements
```python
# requirements.txt for tools
click>=8.0.0          # Command-line interface
pyyaml>=6.0           # YAML configuration parsing
jinja2>=3.0           # Template generation
protobuf>=4.0         # Protocol Buffer support
grpcio-tools>=1.50    # gRPC code generation
docker>=6.0           # Docker management
kubernetes>=25.0      # Kubernetes deployment
psycopg2>=2.9         # PostgreSQL database
redis>=4.5            # Redis cache management
pytest>=7.0           # Testing framework
coverage>=7.0         # Code coverage
black>=22.0           # Code formatting
mypy>=1.0             # Type checking
```

### System Dependencies
- Python 3.8+
- CMake 3.25+
- Docker and Docker Compose
- kubectl (for Kubernetes deployment)
- clang-format (for C++ formatting)

## Usage Examples

### Daily Development Workflow
```bash
# Start development session
atlas setup
atlas build
atlas dev

# Make changes, then test
atlas test --service atlas
atlas format
atlas validate

# Generate updated documentation
atlas generate docs

# Deploy to staging for testing
atlas deploy staging
```

### CI/CD Pipeline Usage
```bash
# CI pipeline
atlas validate
atlas build --release
atlas test --coverage
atlas docker build
atlas docker push

# CD pipeline
atlas deploy production --tag latest
atlas db migrate
atlas status
```

### Adding New Service
```bash
# Generate service skeleton
atlas generate service --name tracking --port 50058

# Generate protocol definitions
atlas generate proto --service tracking

# Build and test
atlas build --service tracking
atlas test --service tracking

# Deploy
atlas deploy --service tracking
```

For detailed information about specific tools, see their individual documentation in the [Developer Guide](../docs/dev/).

## Contributing

When adding new tools:
1. Follow the existing command structure and patterns
2. Add comprehensive tests
3. Include help text and examples
4. Update this README with new functionality
5. Consider backward compatibility with existing workflows
