#pragma once

#include <QString>
#include <QColor>
#include <QDateTime>
#include <QPointF>
#include <QIcon>

#include <array>
#include <cstdint>
#include <functional>
#include <vector>

namespace UI::Types
{
// =================================
// Core Enums
// =================================

enum class Theme
{
    Light,
    Dark,
    HighConstrast,
    Custom
};

enum class Style
{
    Modern,
    Defense,
    Aviation,
    Maritime
};

enum class Priority
{
    Low,
    Normal,
    High,
    Critical,
    Emergency
};

enum class ConnectionStatus
{
    Disconnected,
    Connecting,
    Connected,
    Reconnecting,
    Error,
    Lost
};

enum class ViewMode
{
    List,
    Grid,
    Tiles,
    Compact,
    Detailed,
    Map
};

enum class SelectionMode
{
    None,
    Single,
    Multiple,
    Extended
};

enum class Size
{
    Small,
    Medium,
    Large,
    Auto
};

enum class Position
{
    Left,
    Right,
    Top,
    Bottom,
    Center
};

// =================================
// Drone Types
// =================================

enum class DroneStatus
{
    Offline,
    Idle,
    Armed,
    TakingOff,
    Flying,
    Hovering,
    Landing,
    Landed,
    Emergency,
    Maintenance,
    Lost,
    Crashed
};

enum class FlightMode
{
    Manual,
    Stablize,
    AltitudeHold,
    PositionHold,
    Guided,
    Auto,
    ReturnToBase,
    Land,
    Follow,
    Circle,
    Loiter
};

enum class DroneCapability : uint32_t
{
    Video = 1,

};

// =================================
// Operation Types
// =================================

// =================================
// UI State Types 
// =================================

// =================================
// Data Structures
// =================================

// =================================
// Helper Functions
// =================================
} // namespace UI::Types
