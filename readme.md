visit - https://akschitra.vercel.app
for all documentaion and information


# AksChitra (Foundation)

This is an experiment to treat the terminal like a rendering surface instead of a text output.

Most programs do:
print → line → scroll → gone

I’m doing:
control → position → color → render

---

## What this is

AksChitra (this repo) is the **foundation version**.

It draws stuff in terminal using:
- cursor movement
- ANSI colors
- spaces as pixels

So yeah, fake pixels.

---

## Why?

Because terminal, GitHub, Obsidian — all of them are just:

> structured text rendering systems

I just pushed that idea further:

> what if text output behaves like a framebuffer?

---

## What it can do

- Draw pixels
- Draw lines
- Draw shapes (square, circle, triangle, etc.)
- Apply RGB / HEX colors
- Render at exact positions

Everything is just:
cursor + " " + background color

---

## What it is NOT

- Not a UI framework  
- Not ncurses alternative  
- Not optimized  
- Not the final version  

---

## Important

This repo is **only the foundation**.

The current version (not here) is:
- frame-based
- layered
- buffered
- way more controlled

This one is just:
> immediate rendering prototype

---

## How it works (simple)

1. Generate positions (shape math)
2. Move cursor to those positions
3. Print colored spaces

That’s it.

---

## Example

```cpp
akschitra_foundation engine;

aksf_origin o;
o.pos = {10, 10};
o.clr = {RAW_RGB, "", "", 255, 0, 0};

engine.pixel(o);
