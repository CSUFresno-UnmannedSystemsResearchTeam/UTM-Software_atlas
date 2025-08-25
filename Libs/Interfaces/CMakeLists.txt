# =============================================================================
# Interfaces Library (Header-only)
# =============================================================================

file(GLOB_RECURSE HEADERS include/*.h)

create_utm_module(utm_interfaces INTERFACE
    HEADER_ONLY
    PUBLIC_DEPS
        Qt6::Core
)
