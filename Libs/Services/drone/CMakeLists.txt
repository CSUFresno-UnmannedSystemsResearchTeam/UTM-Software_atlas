# =============================================================================
# Drone Service Module
# =============================================================================

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

create_utm_module(utm_drone_service STATIC
    HEADERS ${HEADERS}
    SOURCES ${SOURCES}
    PUBLIC_DEPS
        Qt6::Core
        utm_interfaces
    PRIVATE_DEPS
        utm_utils
        utm_database
        utm_communication
        utm_events
        Qt6::Concurrent
    DEFINITIONS
        $<$<CONFIG:Debug>:DRONE_SERVICE_DEBUG>
)
