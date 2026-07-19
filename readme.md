# Read it first
v3 is in progress - use legacy till then.

website is available for this project: https://akschitra.vercel.app/

edit: website is currently broken due to some reasons but shares a necessary information about creator and project.

# AksChitra

AksChitra is a terminal rendering framework written in C++.

The project explores a simple idea:

> Treat the terminal as a rendering surface instead of a text output device.

Rather than focusing on lines, streams, and scrolling text, AksChitra focuses on position, state, color, and rendering.

---

## Project Structure

AksChitra is divided into two branches of development.

### Initiality (Opensource)

Initiality is the minimal core.

Its purpose is to provide a small, stable, and understandable rendering foundation.

Focus:

* Minimal API
* Cross-platform support
* Terminal framebuffer concepts
* Simplicity
* Learning and experimentation

Initiality serves as the base layer for everything built on top of AksChitra.

---

### Advanced (Private)

Advanced expands on the Initiality core.

Its purpose is to explore more complex rendering systems, optimizations, and higher-level functionality.

Focus:

* Rendering optimizations
* Layer systems
* Advanced rendering techniques
* Experimental features
* Future extensions

Advanced is intended to evolve over time while remaining rooted in the Initiality philosophy.

---

## Philosophy

Most terminal applications operate through text flow:

```text
Print
→
Scroll
→
Repeat
```

AksChitra approaches the terminal differently:

```text
State
→
Position
→
Render
```

The goal is not to imitate a graphical windowing system.

The goal is to investigate how far terminal rendering can be pushed while remaining simple and understandable.

---

## Design Goals

* Small core
* Cross-platform compatibility
* Direct control over rendering
* Minimal abstraction
* Educational value
* Extensibility

---

## Repository Layout

```text
AksChitra/

├── Docs/
├── Examples/
├── include/
├── legacy/
├── LICENSE
├── readme.md
└── useCases_readme.md
```

---

## Current Status

The repository currently contains the Initiality core implementations and supporting material.

Advanced development will continue as separate extensions built upon the Initiality concepts.

---

## Vision

AksChitra began as an experiment.

The long-term goal is to build a collection of terminal rendering technologies that remain simple enough to understand, modify, and extend while still being capable of producing complex visual output.

---

## License

GPL-3.0
