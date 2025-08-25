# =============================================================================
# Communication Core Module (uses pImpl for complex dependencies)
# =============================================================================

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)

create_utm_module(utm_communication STATIC
    HEADERS ${HEADERS}
    SOURCES ${SOURCES}
    PUBLIC_DEPS
        Qt6::Core
        Qt6::Network
    PRIVATE_DEPS
        utm_utils
        utm_events
        ${GNURADIO_LIBRARIES}
        gnuradio::gnuradio-iio
        gnuradio::gnuradio-blocks
        gnuradio::gnuradio-digital
        gnuradio::gnuradio-filter
        Boost::system
        Boost::thread
        ${OpenCV_LIBS}
        ${FFMPEG_LIBRARIES}
    DEFINITIONS
        $<$<CONFIG:Debug>:COMM_DEBUG_MODE>
        MAVLINK_COMM_NUM_BUFFERS=4
        MAVLINK_USE_CXX_NAMESPACE
        USE_ADALM_PLUTO_SDR
        TDD_MODE
)

# Additional private includes for MAVLink and GNU Radio
target_include_directories(utm_communication
    PRIVATE
        ${MAVLINK_INCLUDE_DIR}
        ${GNURADIO_INCLUDE_DIRS}
        ${FFMPEG_INCLUDE_DIRS}
)
