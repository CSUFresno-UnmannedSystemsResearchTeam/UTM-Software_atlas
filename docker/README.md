# ATLAS Docker Configurations

This directory contains all Docker-related files for building, developing, and deploying the ATLAS UTM system across different environments.

## Overview

ATLAS uses containerization for:
- **Consistent development environments** across team members
- **Simplified deployment** to various environments
- **Service isolation** and dependency management
- **Scalable production deployment**

## Structure

```
docker/
├── dev/                    # Development environment
│   ├── docker-compose.yml  # Development services
│   ├── Dockerfile.atlas-dev # Pre-built development image
│   └── README.md           # Development setup guide
├── prod/                   # Production environment
│   ├── docker-compose.yml  # Production services
│   ├── Dockerfile.atlas    # Production application image
│   ├── Dockerfile.nginx    # Reverse proxy configuration
│   └── README.md           # Production deployment guide
├── ci/                     # Continuous Integration
│   ├── Dockerfile.ci       # CI/testing environment
│   ├── test-compose.yml    # Testing services
│   └── README.md           # CI setup documentation
└── scripts/                # Docker utility scripts
    ├── build.sh            # Build all images
    ├── push.sh             # Push images to registry
    ├── deploy.sh           # Deployment automation
    └── cleanup.sh          # Clean up resources
```

## Environment Configurations

### **dev/** - Development Environment

**Purpose**: Local development with hot-reloading and debugging capabilities

**Key Features**:
- Pre-built development image with all tools installed
- Source code mounted as volumes for live editing
- Development databases (PostgreSQL + Redis)
- Debug ports exposed
- Persistent build cache for faster rebuilds

**Quick Start**:
```bash
cd docker/dev
docker-compose up -d
docker exec -it atlas-dev bash
```

**Services**:
- `atlas-dev`: Development container with pre-installed tools
- `postgres`: Development database
- `redis`: Development cache/message broker

### **prod/** - Production Environment

**Purpose**: Production deployment with optimized performance and security

**Key Features**:
- Multi-stage builds for minimal image sizes
- Health checks and restart policies
- Resource limits and scaling configuration
- TLS/SSL termination
- Logging and monitoring integration

**Services**:
- `atlas-gateway`: Nginx reverse proxy and load balancer
- `atlas-app`: Main ATLAS application
- `protocol-service`: Protocol handling service
- `mission-service`: Mission planning service
- `geofencing-service`: Airspace management service
- `telemetry-service`: Real-time data processing
- `alert-service`: Notification management
- `radio-service`: Hardware communication
- `database-service`: Data persistence
- `postgres`: Production database with persistence
- `redis`: Production cache with clustering

### **ci/** - Continuous Integration

**Purpose**: Automated testing and validation in CI/CD pipelines

**Key Features**:
- Lightweight images for fast CI execution
- All testing dependencies included
- Parallel test execution support
- Code coverage collection
- Artifact generation

**Services**:
- `atlas-ci`: Testing environment
- `test-postgres`: Test database
- `test-redis`: Test cache

## Image Types

### **Development Images**

#### **atlas-dev** (`Dockerfile.atlas-dev`)
- **Base**: Ubuntu 22.04
- **Size**: ~2GB (includes all development tools)
- **Purpose**: Complete development environment
- **Includes**:
  - Qt 6 development stack
  - CMake, Ninja build system
  - vcpkg with pre-installed packages
  - Python development environment
  - Database client libraries
  - Debugging tools (GDB, Valgrind)
  - Code quality tools (clang-format, cppcheck)

```dockerfile
# Example usage
FROM ghcr.io/csufresno-unmannedsystemsresearchteam/atlas-dev:latest
WORKDIR /workspace
# Development tools are already installed
```

### **Production Images**

#### **atlas-app** (`Dockerfile.atlas`)
- **Base**: Ubuntu 22.04 (multi-stage build)
- **Size**: ~500MB (runtime only)
- **Purpose**: Main ATLAS application for production
- **Optimizations**:
  - Multi-stage build (build stage + runtime stage)
  - Only runtime dependencies included
  - Non-root user execution
  - Health checks included

#### **atlas-services** (Individual service Dockerfiles)
- **Base**: Ubuntu 22.04 minimal
- **Size**: ~200MB each (microservice runtime)
- **Purpose**: Individual gRPC services
- **Features**:
  - Service-specific dependencies only
  - Health check endpoints
  - Configuration via environment variables
  - Graceful shutdown handling

#### **nginx-proxy** (`Dockerfile.nginx`)
- **Base**: nginx:alpine
- **Size**: ~50MB
- **Purpose**: Reverse proxy and load balancer
- **Features**:
  - SSL/TLS termination
  - Request routing to services
  - Rate limiting
  - Static file serving

### **CI/Testing Images**

#### **atlas-ci** (`Dockerfile.ci`)
- **Base**: Ubuntu 22.04
- **Size**: ~1.5GB (testing tools included)
- **Purpose**: Automated testing in CI/CD
- **Includes**:
  - All development dependencies
  - Testing frameworks
  - Code coverage tools
  - Performance testing utilities

## Docker Compose Configurations

### **Development Compose** (`dev/docker-compose.yml`)

```yaml
# Key features of development compose
services:
  atlas-dev:
    image: ghcr.io/csufresno-unmannedsystemsresearchteam/atlas-dev:latest
    volumes:
      - ../..:/workspace          # Source code mounting
      - build_cache:/workspace/build  # Persistent build cache
    environment:
      - ATLAS_ENV=development
      - DATABASE_URL=postgresql://atlas:atlas@postgres:5432/atlas_dev
    ports:
      - "8080:8080"              # Application UI
      - "50051-50056:50051-50056" # gRPC services
      - "5678:5678"              # Python debugger
```

### **Production Compose** (`prod/docker-compose.yml`)

```yaml
# Key features of production compose
services:
  atlas-gateway:
    image: atlas-nginx:latest
    ports:
      - "80:80"
      - "443:443"
    depends_on:
      - atlas-app
  
  atlas-app:
    image: atlas-app:latest
    deploy:
      replicas: 3              # Load balancing
      resources:
        limits:
          memory: 512M         # Resource constraints
          cpus: '0.5'
    healthcheck:
      test: ["CMD", "curl", "-f", "http://localhost:8080/health"]
      interval: 30s
      timeout: 10s
      retries: 3
```

### **CI Compose** (`ci/test-compose.yml`)

```yaml
# Key features of CI compose
services:
  atlas-ci:
    image: atlas-ci:latest
    environment:
      - CI=true
      - ATLAS_ENV=testing
    volumes:
      - ../..:/workspace
      - test_artifacts:/workspace/test-results
```

## Scripts and Automation

### **scripts/build.sh** - Build All Images
```bash
#!/bin/bash
# Builds all Docker images for ATLAS

./scripts/build.sh                    # Build all images
./scripts/build.sh --env dev          # Build development images only
./scripts/build.sh --env prod         # Build production images only
./scripts/build.sh --push             # Build and push to registry
```

**Features**:
- Parallel image building for speed
- Dependency-aware build order
- Build caching optimization
- Multi-platform builds (AMD64, ARM64)
- Automatic tagging with git commit SHA

### **scripts/push.sh** - Push Images to Registry
```bash
#!/bin/bash
# Pushes built images to container registry

./scripts/push.sh                     # Push all images
./scripts/push.sh --tag latest        # Push with specific tag
./scripts/push.sh --registry ghcr.io  # Push to specific registry
```

### **scripts/deploy.sh** - Deployment Automation
```bash
#!/bin/bash
# Deploys ATLAS to various environments

./scripts/deploy.sh local             # Deploy locally
./scripts/deploy.sh staging           # Deploy to staging
./scripts/deploy.sh production        # Deploy to production
./scripts/deploy.sh --rollback        # Rollback last deployment
```

**Features**:
- Environment-specific configurations
- Health check validation
- Rolling deployment support
- Automatic rollback on failure
- Database migration handling

### **scripts/cleanup.sh** - Resource Cleanup
```bash
#!/bin/bash
# Cleans up Docker resources

./scripts/cleanup.sh                  # Clean unused resources
./scripts/cleanup.sh --deep           # Deep clean (removes all ATLAS images)
./scripts/cleanup.sh --volumes        # Clean persistent volumes (data loss!)
```

## Registry and Image Management

### **GitHub Container Registry**
ATLAS uses GitHub Container Registry (ghcr.io) for image distribution:

```bash
# Image naming convention
ghcr.io/csufresno-unmannedsystemsresearchteam/atlas-dev:latest
ghcr.io/csufresno-unmannedsystemsresearchteam/atlas-app:v1.0.0
ghcr.io/csufresno-unmannedsystemsresearchteam/atlas-protocol:latest
```

### **Image Tagging Strategy**
- `latest`: Latest stable build from main branch
- `v1.0.0`: Semantic version releases
- `main-abc123`: Branch name + commit SHA
- `pr-42`: Pull request builds
- `nightly`: Automated nightly builds

### **Authentication**
```bash
# Login to GitHub Container Registry
echo $GITHUB_TOKEN | docker login ghcr.io -u USERNAME --password-stdin

# Or using GitHub CLI
gh auth token | docker login ghcr.io -u USERNAME --password-stdin
```

## Development Workflow

### **Daily Development**
```bash
# 1. Start development environment
cd docker/dev
docker-compose up -d

# 2. Enter development container
docker exec -it atlas-dev bash

# 3. Develop and test
./atlas build
./atlas test

# 4. View logs
docker-compose logs -f
```

### **Testing Changes**
```bash
# Test locally built image
docker build -f docker/dev/Dockerfile.atlas-dev -t atlas-dev-local .
docker run -it atlas-dev-local bash

# Test with CI environment
cd docker/ci
docker-compose up --build
```

### **Preparing for Production**
```bash
# Build production images
./docker/scripts/build.sh --env prod

# Test production deployment locally
cd docker/prod
docker-compose up -d

# Run production health checks
./atlas status
```

## Performance and Optimization

### **Image Size Optimization**
- Multi-stage builds to minimize production images
- `.dockerignore` files to exclude unnecessary files
- Layer caching optimization
- Package cleanup in build stages

### **Build Speed Optimization**
- BuildKit for advanced caching
- Parallel builds across services
- Registry layer caching
- Local build cache persistence

### **Runtime Performance**
- Resource limits and requests
- Health checks for container orchestration
- Graceful shutdown handling
- Logging configuration for performance

## Security Considerations

### **Image Security**
- Base images updated regularly
- Vulnerability scanning in CI
- Non-root user execution
- Minimal attack surface in production images

### **Runtime Security**
- Read-only file systems where possible
- Secret management via environment variables
- Network segmentation between services
- TLS encryption for inter-service communication

## Monitoring and Logging

### **Container Monitoring**
```bash
# View container resource usage
docker stats

# Monitor specific service
docker-compose logs -f atlas-app

# Export logs for analysis
docker-compose logs > atlas-logs.txt
```

### **Health Checks**
All production containers include health checks:
```dockerfile
HEALTHCHECK --interval=30s --timeout=10s --retries=3 \
  CMD curl -f http://localhost:8080/health || exit 1
```

### **Log Aggregation**
Production deployments can integrate with:
- ELK Stack (Elasticsearch, Logstash, Kibana)
- Prometheus and Grafana
- Docker logging drivers
- Cloud provider logging services

## Troubleshooting

### **Common Issues**

**Container won't start:**
```bash
# Check container logs
docker-compose logs service-name

# Inspect container configuration
docker inspect container-name

# Check resource usage
docker stats
```

**Build failures:**
```bash
# Clean build cache
docker builder prune

# Rebuild without cache
docker-compose build --no-cache

# Check disk space
df -h
```

**Networking issues:**
```bash
# List Docker networks
docker network ls

# Inspect network configuration
docker network inspect atlas-development

# Test connectivity between containers
docker exec -it container1 ping container2
```

**Permission issues:**
```bash
# Check file ownership in mounted volumes
docker exec -it atlas-dev ls -la /workspace

# Fix ownership (if needed)
sudo chown -R $USER:$USER .
```

### **Debugging Tips**

**Interactive debugging:**
```bash
# Run container with shell for debugging
docker run -it --rm atlas-dev:latest bash

# Override entrypoint for troubleshooting
docker run -it --rm --entrypoint bash atlas-app:latest

# Mount current directory for testing
docker run -it --rm -v $(pwd):/workspace atlas-dev:latest
```

**Performance debugging:**
```bash
# Monitor resource usage
docker stats --no-stream

# Check container processes
docker exec -it atlas-dev ps aux

# View container filesystem
docker exec -it atlas-dev df -h
```

## Best Practices

### **Development**
- Use `.dockerignore` to exclude unnecessary files
- Keep Dockerfiles in the same directory as related code
- Use multi-stage builds for production images
- Pin base image versions for reproducibility

### **Production**
- Always use specific image tags, never `latest` in production
- Implement proper health checks
- Set resource limits to prevent resource exhaustion
- Use secrets management for sensitive data

### **CI/CD**
- Build images in CI for consistency
- Run security scans on all images
- Use registry webhooks for automated deployments
- Implement proper rollback procedures

For environment-specific documentation, see:
- [Development Setup](dev/README.md)
- [Production Deployment](prod/README.md)
- [CI Configuration](ci/README.md)
