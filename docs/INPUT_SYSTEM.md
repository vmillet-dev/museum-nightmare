# Input Management System

## Overview
The input management system provides a flexible architecture for handling both player and AI-controlled actors in the game. It supports keyboard and controller input for human players, with hot-plugging capabilities, and various AI behaviors for enemy actors.

## Architecture

### Actor System
- `Actor`: Base class for all input-driven game objects
- `Player`: Human-controlled actor
- `Enemy`: AI-controlled actor

### Input Providers
- `IInputProvider`: Abstract interface for input handling
- `HumanInputProvider`: Handles keyboard and controller input
- `AIInputProvider`: Manages AI behavior patterns

### State Machine
- `ActorState`: Base class for actor states
- Supported States:
  - Idle
  - Walking
  - Jumping
  - Attacking
  - Stunned
  - Dead

## Configuration
Input bindings and actor properties are configured in `assets/config.toml`:

```toml
[controls]
# Keyboard bindings
move_up = "Z"
move_down = "S"
move_left = "Q"
move_right = "D"
jump = "Space"
attack = "E"
interact = "F"

# Controller settings
controller_enabled = true
controller_type = "xbox"  # or "playstation"
controller_deadzone = 20.0
controller_sensitivity = 100.0
```

## Controller Support
- Supports both Xbox and PlayStation controllers
- Hot-plugging: Controllers can be connected/disconnected during gameplay
- Automatic controller type detection
- Configurable deadzones and sensitivity

## AI Behaviors
Available AI behaviors for enemy actors:
- `Patrol`: Move between points
- `Chase`: Follow target
- `Wander`: Random movement
- `Idle`: Stand still

## Testing
Run tests using:
```bash
cd build
cmake ..
make
ctest --output-on-failure
```

Note: Controller tests require manual verification with physical hardware.

## Example Usage

### Creating a Player
```cpp
auto player = std::make_unique<Player>(400.f, 300.f);
```

### Creating an Enemy
```cpp
auto enemy = std::make_unique<Enemy>(400.f, 300.f, AIBehaviorType::Patrol);
enemy->setTarget(playerPosition);  // For chase behavior
```
