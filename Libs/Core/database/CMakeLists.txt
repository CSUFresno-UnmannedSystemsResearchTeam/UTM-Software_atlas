# =============================================================================
# Database Core Module
# =============================================================================

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

create_utm_module(utm_database STATIC
    HEADERS ${HEADERS}
    SOURCES ${SOURCES}
    PUBLIC_DEPS
        Qt6::Core
        Qt6::Sql
        SQLite::SQLite3
    PRIVATE_DEPS
        utm_utils
    DEFINITIONS
        $<$<CONFIG:Debug>:DATABASE_DEBUG_MODE>
)
