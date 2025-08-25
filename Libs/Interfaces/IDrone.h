#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Interface
{
namespace Drone
{

// I am unsure aboot all of these
enum class Capability
{
    Video = 1,
    Thermal,
    Infared,
    SearchLight,
    Speaker,
    PayloadBay,     // Ability to drop items from a drone
    HighAltitude,
    LongRange,
    FastSpeed,
    HeavyLift
};

// I am unsure abouth these
enum class Status
{
    Offline,
    Idle,
    Armed,
    TakingOff,
    InFlight,
    Landing,
    Landed,
    Emergency,
    Maintenance
};

// Unsure about all these
enum class FlightMode
{
    Manual,     // Implies some algorithmic control when false
    Stabilize,
    AltitudeHold,
    PositionHold,
    Guided,
    Auto,
    ReturnToLaunch,
    Land,
    Follow,
    Circle
};

struct Positioning_t
{
    double latitude {0};
    double longitude {0};
    double absoluteAltitude {0};
    double relativeAltitude {0};
};
/*
    @brief Rotational orientation in a body-fixed frame.
    @see SHALL adhere to the following:
        * Units MUST be in radians
        * ISO 80000-3
        * ISO 1151
        * ZYX Euler angle
    @param roll: rotation about x-axis
        * Positive = right side down
        * Negative = right side up
    @param pitch: rotation about y-axis
        * Positive = nose up
        * Negative = nose down
    @param yaw: rotation about z-axis
        * Positive = clockwise
        * Negative = counter-clockwise
*/
struct Rotation_t
{
    double roll{0.0};   // radians
    double pitch{0.0};  // radians
    double yaw{0.0};    // radians
};
/*
    @brief Translation velocities in a body-fixed frame.
    @see SHALL adhere to the following:
        * Units MUST be in meters per seconds (m/s)
        * ISO 80000-3
        * ISO 1151
    @param x: velocity along x-axis
        * Positive = forward
        * Negative = backward
    @param y: velocity along y-axis
        * Positive = right
        * Negative = left
    @param z: velocity along z-axis
        * Positive = down
        * Negative = up
*/
struct Translation_t
{
    double x{0.0};  // meters per second
    double y{0.0};  // meters per second
    double z{0.0};  // meters per second
};

struct TelemetryData_t
{
    
};

} // namespace Drone
} // namespace Interface
