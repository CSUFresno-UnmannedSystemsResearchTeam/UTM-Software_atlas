# ATLAS Documentation

This directory contains comprehensive documentation for the ATLAS UTM (Autonomous Traffic and Logistic Alignment Systems) project.

## Overview

The ATLAS documentation is organized by audience and purpose to help different types of users find the information they need quickly and efficiently.

## Structure

```
docs/
├── api/                    # API documentation and references
│   ├── grpc_services/     # gRPC service API documentation
│   ├── rest_endpoints/    # REST API documentation
│   ├── protocols/         # Protocol specifications (MAVLink, custom)
│   └── sdk/              # SDK API documentation
├── arch/                  # Architecture and design documentation
│   ├── overview.md       # High-level system architecture
│   ├── services.md       # Service design and interactions
│   ├── database.md       # Database schema and design
│   ├── security.md       # Security architecture
│   └── deployment.md     # Deployment architecture
├── dev/                   # Developer documentation
│   ├── getting-started.md # Quick start for new developers
│   ├── contributing.md   # Contribution guidelines
│   ├── coding-standards.md # Code style and standards
│   ├── testing.md        # Testing guidelines
│   ├── debugging.md      # Debugging and troubleshooting
│   └── release-process.md # Release and deployment process
└── user/                  # End-user documentation
    ├── installation.md   # Installation and setup
    ├── quick-start.md    # Quick start guide for operators
    ├── user-guide.md     # Comprehensive user manual
    ├── tutorials/        # Step-by-step tutorials
    ├── troubleshooting.md # Common issues and solutions
    └── faq.md           # Frequently asked questions
```

## Documentation by Audience

### **For New Developers** 👨‍💻
Start here to get up and running quickly:

1. **[Getting Started](dev/getting-started.md)** - Complete setup guide
2. **[Architecture Overview](arch/overview.md)** - Understand the system design
3. **[Contributing Guidelines](dev/contributing.md)** - How to contribute effectively
4. **[Coding Standards](dev/coding-standards.md)** - Code style and best practices

**Essential reading for developers:**
- [Service Architecture](arch/services.md) - Understanding the microservices design
- [Testing Guidelines](dev/testing.md) - How to write and run tests
- [API Documentation](api/) - Service interfaces and protocols

### **For System Architects** 🏗️
Deep dive into system design and architecture:

1. **[System Architecture](arch/overview.md)** - High-level system design
2. **[Service Design](arch/services.md)** - Microservices architecture
3. **[Database Design](arch/database.md)** - Data architecture and schemas
4. **[Security Architecture](arch/security.md)** - Security design and considerations
5. **[Deployment Architecture](arch/deployment.md)** - Infrastructure and deployment

### **For Operators** 🚁
Learn how to use ATLAS for drone operations:

1. **[Quick Start Guide](user/quick-start.md)** - Get started quickly
2. **[User Guide](user/user-guide.md)** - Comprehensive operator manual
3. **[Tutorials](user/tutorials/)** - Step-by-step operational procedures
4. **[Troubleshooting](user/troubleshooting.md)** - Solving common problems

### **For API Consumers** 📡
Integration and SDK usage:

1. **[SDK Documentation](api/sdk/)** - Drone developer SDK
2. **[gRPC Services](api/grpc_services/)** - Service-to-service APIs
3. **[REST API](api/rest_endpoints/)** - External integration APIs
4. **[Protocol Specifications](api/protocols/)** - Communication protocols

### **For DevOps Engineers** ⚙️
Deployment and operational concerns:

1. **[Installation Guide](user/installation.md)** - System installation
2. **[Deployment Architecture](arch/deployment.md)** - Infrastructure design
3. **[Release Process](dev/release-process.md)** - Deployment procedures
4. **[Debugging Guide](dev/debugging.md)** - Operational troubleshooting

## Quick Navigation

### **Getting Started Paths**

#### **New Team Member (Developer)**
```
1. dev/getting-started.md          # Setup environment
2. arch/overview.md                # Understand architecture  
3. dev/contributing.md             # Learn contribution process
4. api/grpc_services/              # Study service APIs
5. Start contributing!
```

#### **New Operator**
```
1. user/installation.md            # Install ATLAS
2. user/quick-start.md             # First mission
3. user/tutorials/basic-mission.md # Learn basic operations
4. user/user-guide.md              # Master all features
```

#### **Integration Developer**
```
1. api/sdk/                        # SDK documentation
2. api/protocols/                  # Protocol specifications
3. dev/testing.md                  # Testing your integration
4. user/tutorials/sdk-integration.md # Integration examples
```

### **Common Tasks**

| Task | Documentation |
|------|---------------|
| Set up development environment | [dev/getting-started.md](dev/getting-started.md) |
| Understand system architecture | [arch/overview.md](arch/overview.md) |
| Add a new service | [dev/contributing.md](dev/contributing.md#adding-new-services) |
| Write tests | [dev/testing.md](dev/testing.md) |
| Deploy to production | [arch/deployment.md](arch/deployment.md) |
| Integrate a new drone | [api/sdk/](api/sdk/) |
| Plan a search mission | [user/tutorials/search-mission.md](user/tutorials/search-mission.md) |
| Troubleshoot issues | [user/troubleshooting.md](user/troubleshooting.md) |

## Documentation Standards

### **Writing Guidelines**

All ATLAS documentation follows these standards:

1. **Clear Structure**: Use consistent headings and organization
2. **Audience-Focused**: Write for the specific audience and use case
3. **Actionable**: Provide concrete steps and examples
4. **Up-to-Date**: Keep documentation current with code changes
5. **Tested**: Verify all examples and procedures work

### **Markdown Standards**

- Use GitHub Flavored Markdown
- Include table of contents for longer documents
- Use code blocks with language specification
- Include screenshots for UI documentation
- Link between related documents

### **Code Examples**

All code examples should be:
- **Complete**: Runnable without additional context
- **Commented**: Explain non-obvious parts
- **Current**: Match the latest codebase
- **Tested**: Verified to work as documented

Example:
```cpp
// Good: Complete, commented example
#include "atlas/mission_service.h"

int main() {
    // Connect to mission service
    auto client = atlas::MissionServiceClient("localhost:50052");
    
    // Create a simple search mission
    atlas::SearchMission mission;
    mission.set_area_center_lat(40.7128);
    mission.set_area_center_lon(-74.0060);
    mission.set_search_radius(1000);  // meters
    
    // Submit mission for execution
    auto response = client.StartMission(mission);
    if (response.success()) {
        std::cout << "Mission started: " << response.mission_id() << std::endl;
    }
    
    return 0;
}
```

## Contributing to Documentation

### **How to Contribute**

1. **Identify gaps**: What documentation is missing or unclear?
2. **Follow templates**: Use existing documents as templates
3. **Write drafts**: Create initial versions in markdown
4. **Get feedback**: Review with team members
5. **Test procedures**: Verify all steps and examples work
6. **Submit PR**: Follow the standard contribution process

### **Documentation Types**

#### **Tutorials** (`user/tutorials/`)
Step-by-step guides for specific tasks:
```markdown
# Tutorial: Setting Up Your First Search Mission

## Prerequisites
- ATLAS installed and running
- At least 3 drones registered

## Step 1: Create Mission
1. Open ATLAS application
2. Navigate to "Operations" tab
3. Click "New Mission"
...
```

#### **How-To Guides** (`dev/`)
Problem-solving focused documentation:
```markdown
# How to Debug Service Communication Issues

## Problem
Services are not communicating properly.

## Diagnosis
1. Check service health: `./atlas status`
2. Verify network connectivity: `./atlas test connectivity`
3. Review logs: `./atlas logs --service protocol`

## Solutions
...
```

#### **Reference Material** (`api/`)
Comprehensive, organized information:
```markdown
# Mission Service API Reference

## StartMission

**Description**: Initiates a new mission with specified parameters.

**Request**: `StartMissionRequest`
- `mission_type`: Mission type (SEARCH, PATROL, DELIVERY)
- `area`: Geographic area definition
- `drones`: List of assigned drone IDs

**Response**: `StartMissionResponse`
- `success`: Operation success status
- `mission_id`: Unique mission identifier
- `error_message`: Error description (if failed)
```

#### **Explanation** (`arch/`)
Understanding-focused documentation:
```markdown
# ATLAS Service Architecture

ATLAS uses a microservices architecture to provide scalability and maintainability. Each service handles a specific domain of functionality and communicates with other services through well-defined APIs.

## Design Principles
1. **Single Responsibility**: Each service has one clear purpose
2. **Loose Coupling**: Services interact through APIs, not direct dependencies
3. **High Cohesion**: Related functionality is grouped together
...
```

## Maintenance

### **Keeping Documentation Current**

1. **Code Reviews**: Documentation changes required for API changes
2. **Release Process**: Documentation updates included in releases
3. **Regular Audits**: Quarterly documentation review and updates
4. **User Feedback**: Incorporate feedback from documentation users

### **Documentation Automation**

- **API Documentation**: Auto-generated from code annotations
- **Change Detection**: Automated alerts for outdated documentation
- **Link Checking**: Automated verification of internal and external links
- **Build Integration**: Documentation builds with code builds

### **Metrics and Feedback**

Track documentation effectiveness through:
- User feedback and surveys
- Documentation usage analytics
- Support ticket analysis
- Developer onboarding feedback

## Support and Help

### **Getting Help with Documentation**

- **GitHub Issues**: Report documentation bugs or gaps
- **GitHub Discussions**: Ask questions about documentation
- **Team Slack/Discord**: Real-time documentation discussions
- **Office Hours**: Regular documentation Q&A sessions

### **Documentation Team**

- **Maintainers**: Overall documentation strategy and quality
- **Contributors**: Subject matter experts for specific areas
- **Reviewers**: Technical accuracy and clarity validation
- **Users**: Feedback providers and improvement suggestions

---

**Remember**: Good documentation is as important as good code. It enables others to understand, use, and contribute to ATLAS effectively.

For questions about documentation, create an issue with the `documentation` label or join our discussions in the #docs channel.
