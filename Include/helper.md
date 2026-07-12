# akschitraDisplay Helper Documentation

## Overview
`akschitraDisplay` is a C++ class for rendering colored characters to the terminal using ANSI escape codes. It creates a grid-based display buffer that maps to terminal rows and columns.

## Class API

### Constructor & Destructor
```cpp
akschitraDisplay()          // Default constructor
~akschitraDisplay()         // Destructor - frees display memory
```

### Public Methods

#### Initialization
```cpp
void Initialise()
```
**What**: Gets terminal dimensions and allocates display buffer  
**How**: Uses `ioctl()` with `TIOCGWINSZ` to get window size  
**When**: Call once before any other operations  
**Why**: Required to set up the display grid; terminal size must be known

---

#### Setting Pixels (Cells)
```cpp
bool setByHex(int row, int col, const std::string& hex_code="#ffffff", std::string character="█")
```
**What**: Sets a cell's color using hex color code  
**How**: Validates hex format (#RRGGBB), converts to RGB, applies ANSI escape sequence  
**When**: When you have a hex color string from a data source  
**Why**: Convenient for web color formats (#FF5733, #00FF00, etc.)

**Parameters**:
- `row` - Row index (0 to rows-1)
- `col` - Column index (0 to cols-1)
- `hex_code` - Hex color string (format: "#ffffff")
- `character` - Character to display (default: "█")

**Returns**: `true` on success, `false` on failure

---

```cpp
bool setByRaw(int row, int col, int r=255, int g=255, int b=255, std::string character="█")
```
**What**: Sets a cell's color using RGB values  
**How**: Validates RGB range (0-255), applies ANSI escape sequence  
**When**: When you have RGB values from calculations or data  
**Why**: More programmatic control; useful for generating colors algorithmically

**Parameters**:
- `row` - Row index (0 to rows-1)
- `col` - Column index (0 to cols-1)
- `r` - Red value (0-255, default: 255)
- `g` - Green value (0-255, default: 255)
- `b` - Blue value (0-255, default: 255)
- `character` - Character to display (default: "█")

**Returns**: `true` on success, `false` on failure

---

#### Getting Pixel Data
```cpp
std::string Get(int row, int col) const
```
**What**: Returns the ANSI string at a specific cell  
**How**: Direct array access with bounds checking  
**When**: For debugging or reading back display data  
**Why**: Allows inspection of current cell contents

**Returns**: ANSI escape string or empty string if invalid

---

#### Rendering
```cpp
bool Render()
```
**What**: Renders the entire display buffer to terminal  
**How**: Builds buffer string, moves cursor to home position, writes all cells  
**When**: After setting all desired cells, call to show the frame  
**Why**: Efficient single write operation to terminal

---

#### Clearing
```cpp
bool Clear()
```
**What**: Clears entire display (terminal screen and buffer)  
**How**: Resets buffer to default spaces, sends clear screen escape codes  
**When**: Before starting new frame or when resetting display  
**Why**: Prevents artifacts from previous renders

---

## Usage Guide

### When to Use Each Method

| Scenario | Method |
|----------|--------|
| Initial setup | `Initialise()` |
| Set colors from web palette | `setByHex()` |
| Generate colors programmatically | `setByRaw()` |
| Display the frame | `Render()` |
| Reset/clear screen | `Clear()` |
| Debug/inspect | `Get()` |

### Complete Example
```cpp
akschitraDisplay display;
display.Initialise();

// Set a red pixel using hex
display.setByHex(5, 10, "#FF0000");

// Set a blue pixel using RGB
display.setByRaw(5, 11, 0, 0, 255);

// Render to terminal
display.Render();
```

## Important Notes

1. **Initialize First**: Always call `Initialise()` before any other operations
2. **Zero-Based**: Row and column indices start from 0
3. **Bounds Check**: Methods validate indices to prevent crashes
4. **Character Choice**: Default is "█" (full block), but you can use any printable character
5. **Terminal Colors**: Works with 24-bit color capable terminals
6. **Memory**: Destructor automatically frees allocated display buffer

## Error Handling

All methods return `false` if:
- Not initialized
- Invalid row/column indices
- Invalid hex code format (for `setByHex`)
- RGB values out of range (for `setByRaw`)

`Get()` returns empty string on failure.