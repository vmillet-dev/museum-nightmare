# Museum Nightmare

A 2D game built with SFML where you navigate through a haunted museum.

## Prerequisites

- CMake (>= 3.16)
- C++23 compatible compiler
- X11 development libraries
- OpenGL development libraries
- OpenAL development libraries
- Vorbis and FLAC audio codecs
- FreeType library

On Ubuntu/Debian, you can install the required dependencies with:
```bash
sudo apt-get update && sudo apt-get install -y \
    build-essential \
    cmake \
    libx11-dev \
    libxrandr-dev \
    libxcursor-dev \
    libgl1-mesa-dev \
    libudev-dev \
    libopenal-dev \
    libvorbis-dev \
    libflac-dev \
    libfreetype-dev
```

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/vmillet-dev/museum-nightmare.git
cd museum-nightmare
```

2. Create and navigate to the build directory:
```bash
mkdir build && cd build
```

3. Configure and build the project:
```bash
cmake ..
make
```

## Running the Game

After building, you can run the game from the build directory:
```bash
./sfml_game
```

## Controls

- Z: Move Up
- S: Move Down
- Q: Move Left
- D: Move Right
- ESC: Exit Game

## Project Structure

The project uses CMake for build configuration and depends on the following libraries (automatically fetched during build):

- SFML 2.6.2: Graphics, window management, and input handling
- toml++ 3.4.0: Configuration file parsing
- spdlog 1.12.0: Logging system
- TGUI 1.6.1: GUI widgets and menu system

## UI System

The game uses TGUI for creating menus and UI elements through a flexible MenuBuilder pattern. Here's how to use it:

### Basic Menu Creation

```cpp
// Create a menu with vertical layout
MenuBuilder menuBuilder;
menuBuilder.setLayout(LayoutType::Vertical)
    .applyResponsive()
    .applyTheme(tgui::Theme("assets/themes/dark.theme"));

// Add widgets with method chaining
menuBuilder.addButton("Play", playCallback)
    .setSize("200", "50")
    .setColor(tgui::Color(100, 100, 100))
    .build()
    .addLabel("Game Title")
    .setSize("400", "50")
    .build();

// Build and add to GUI
menuBuilder.build(gui);
```

### Layout Types

The MenuBuilder supports three layout types:
- `LayoutType::Vertical`: Arranges widgets vertically with automatic spacing
- `LayoutType::Horizontal`: Arranges widgets horizontally with automatic spacing
- `LayoutType::Grid`: Arranges widgets in a grid pattern (coming soon)

### Theme Support

Apply consistent themes across your menus:
```cpp
// Apply theme from file
menuBuilder.applyTheme(tgui::Theme("assets/themes/dark.theme"));

// Theme files support customization of:
// - Button colors and styles
// - Label text properties
// - Widget spacing and sizes
// - Hover and focus effects
```

### Event Handling

Bind callbacks to widget events:
```cpp
menuBuilder.addButton("Start", [this]() {
    // Button click handler
    game.startGame();
})
.setSize("200", "50")
.build();

// Support for various events:
// - onClick
// - onHover
// - onFocus
// - onTextChange (for text inputs)
```

### Responsive Layout

Enable responsive layout to automatically adjust widget positions:
```cpp
menuBuilder.setLayout(LayoutType::Vertical)
    .applyResponsive()  // Enables responsive mode
    .build(gui);
```

### Debug Logging

Enable detailed logging for debugging:
```cpp
menuBuilder.enableLogging(true)  // Logs widget creation and layout
    .build(gui);
```

- The game requires a working X11 display server to run
- Controller support is available and can be configured in the game settings
