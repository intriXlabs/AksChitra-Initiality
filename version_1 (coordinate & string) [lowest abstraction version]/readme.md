# akschitra Display

A minimal, zero-dependency terminal buffer renderer using ANSI escape codes.

## Overview

Terminal Display maintains an in-memory grid of styled cells and renders them to the terminal in a single write operation. Each cell holds an ANSI escape sequence, allowing per-character styling with full terminal color and attribute support.

## How It Works

The library operates in three distinct phases:

### 1. Initialization
- Queries the terminal dimensions via `ioctl(TIOCGWINSZ)`
- Allocates a flat array sized `rows × cols`
- Seeds every cell with the ANSI reset sequence `\033[0m` followed by a space

### 2. State Manipulation
- `Set(row, col, ansi_code)` writes an ANSI escape sequence to a specific cell position
- Bounds checking prevents out-of-range writes
- Cells are simply overwritten — no diffing or dirty tracking occurs

### 3. Render
- Concatenates all cells into a single string buffer
- Prepends `\033[H` (cursor home) to overwrite the previous frame in-place
- Issues one `write()` syscall to stdout, minimizing I/O overhead

## Core Abstractions

| Concept | Implementation |
|---------|---------------|
| **Framebuffer** | Flat `std::string` array indexed as `[row * cols + col]` |
| **Cell** | A complete ANSI sequence per character position |
| **Render cycle** | Full-screen overwrite via cursor-home + single write |
| **Lifetime** | RAII — constructor allocates, destructor frees |

## Mechanisms

- **Double-buffering alternative**: The single buffer approach avoids memory duplication. The `\033[H` escape ensures the terminal overwrites rather than scrolls.
- **Flat array indexing**: `row * cols + col` linearization avoids pointer-to-pointer indirection and improves cache locality during render.
- **Single syscall rendering**: Building the entire frame into one string before calling `write()` reduces context switches and eliminates flicker.
- **Stateless terminal interaction**: No termcap/terminfo dependency. Works on any ANSI-compatible terminal.

## When to Use

- **Minimal TUI applications** that don't need input handling or window management
- **Full-screen animated displays** (dashboards, visualizers, game boards)
- **Embedded or constrained environments** where linking ncurses is undesirable
- **Learning/teaching** terminal rendering fundamentals
- **Prototyping** before committing to a heavier TUI framework

## When to Avoid

- **Complex form-based UIs** requiring input fields, scrolling regions, or window management
- **Applications needing portability** to non-ANSI terminals (Windows console without ANSI support, legacy hardware terminals)
- **Signal handling requirements** — no built-in terminal state restoration on unexpected exit
- **Partial screen updates** — always renders the full terminal, wasteful for sparse changes

## Comparison

### vs. ncurses

| Aspect | Terminal Display | ncurses |
|--------|-----------------|---------|
| **Dependency** | None (C++ standard library only) | libncurses system library |
| **Binary size** | ~2-5 KB compiled | ~200+ KB linked |
| **Input handling** | None | Keyboard, mouse, function keys |
| **Terminal database** | Not required | terminfo/termcap needed |
| **Portability** | ANSI terminals only | Broad terminal support |
| **Partial updates** | Full frame always | Optimized delta rendering |
| **Color pairs** | Unlimited (raw ANSI) | Limited to predefined pairs |
| **Window management** | Manual coordinate math | Built-in WINDOW abstraction |
| **Signal safety** | Manual cleanup required | Automatic terminal restoration |
| **Learning curve** | Trivial | Moderate |

**Where Terminal Display wins:**
- Compilation simplicity — no external linkage, no pkg-config, no cross-compilation headaches
- Cell-level styling granularity — arbitrary ANSI sequences without abstraction layers
- Predictable performance — single allocation, single write, no hidden complexity
- Codebase transparency — the entire implementation is immediately readable

**Where Terminal Display loses:**
- No input abstraction — raw terminal input requires separate handling
- No optimized delta rendering — always redraws everything
- No terminal capability abstraction — fails silently on unsupported terminals
- No curses-style windows, pads, or panels

### vs. notcurses

| Aspect | Terminal Display | notcurses |
|--------|-----------------|-----------|
| **Multimedia** | Text only | Images, video, pixel graphics |
| **Thread safety** | Not considered | Fully thread-safe |
| **Render model** | Immediate full-frame | Retained scene graph |
| **Framerate** | Limited by terminal speed | Optimized with damage tracking |
| **Complexity** | Minimal | Substantial API surface |

**Where Terminal Display wins:**
- Zero cognitive overhead — the entire API is four methods
- No build system integration beyond a single header
- Direct ANSI control without abstraction impedance mismatch

**Where Terminal Display loses:**
- No Sixel/Kitty image protocol support
- No multi-threaded rendering
- No fade, rotation, or alpha blending
- No truecolor detection or graceful degradation

## Usage Example

```cpp
Terminal_Display display;
display.Initialise();

// Red background for top row
for (int col = 0; col < /* width */; col++) {
    display.Set(0, col, "\033[41m ");
}

// Green text at center
display.Set(12, 40, "\033[32mH\033[0m");
display.Set(12, 41, "\033[32me\033[0m");
display.Set(12, 42, "\033[32ml\033[0m");
display.Set(12, 43, "\033[32ml\033[0m");
display.Set(12, 44, "\033[32mo\033[0m");

display.Render();
