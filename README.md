# Block Chunk Generator Assignment - Programming Course

## 📘 Overview

This project is part of the **Programming** course at the Faculty of Computer Science, University Politehnica of Bucharest. The assignment involves manipulating 3D structures (chunks) composed of blocks, implementing a series of tasks inspired by voxel-based environments (e.g., Minecraft).

## 🧱 Problem Context

Each task adds a new functionality for modifying or analyzing a 3D chunk (a fixed-size 3D grid of blocks). The project is implemented in C and emphasizes correct memory management, efficient traversal, and logical structuring of 3D data.

---

## ✅ Task Summary

### Task 1 – Set Single Block
- Checks if the target coordinates are within bounds.
- If valid, updates the chunk at the specified location.

### Task 2 – Set Rectangular Area
- Normalizes coordinate order.
- Iterates through the resulting cuboid and updates all contained blocks.

### Task 3 – Fill Sphere
- Computes the bounding cuboid for a sphere centered at a given point.
- Updates only the blocks strictly within the sphere using the Euclidean distance formula.

### Task 4 – Build Shell Around Block
- Iterates through all neighboring positions using 3D offset vectors.
- Uses a temporary 3D array to avoid placing shell blocks over newly placed ones, especially when `shell_block` equals `target_block`.

### Task 5 – 2D Flood Fill (XZ Plane)
- Implements a fill algorithm using a queue and a 2D visited map (Y axis ignored).
- Adds adjacent blocks that meet fill conditions.

### Task 6 – 3D Flood Fill
- Extends the 2D fill logic to three dimensions.
- Maintains a 3D visited array to track progress.

### Task 7 – Rotate Chunk 90° (around Y-axis)
- Allocates a new chunk rotated in the XZ plane using the mapping:  
  `(x, y, z) → (z, y, width - x - 1)`
- Frees the original chunk and returns the rotated one.

### Task 8 – Apply Gravity
- Implements a gravity simulation by identifying “bodies” of connected blocks and moving them down.
- Two types of bodies:
  - **Uniform bodies**: composed of the same block type.
  - **Mixed bodies**: composed of adjacent blocks regardless of type.
- Uses helper functions:
  - `get_body_at`, `get_mixed_body_at`: extract 3D bodies.
  - `is_touching_ground`: checks if the body is supported from below.
  - `lower_body`: applies downward movement.

### Task 9 – Run-Length Encoding
- Encodes each row of the chunk by grouping identical consecutive blocks.
- Uses bit manipulation to store block type and run length efficiently.

### Task 10 – Run-Length Decoding
- Reconstructs the original chunk by decoding each run from the encoded format.
- Mirrors the logic from Task 9 in reverse.

---

## 🛠 Technologies Used

- **Language**: C
- **Core Techniques**: 3D matrix manipulation, BFS-style fill, geometry calculations, bit-level encoding

---

## 💡 Notes

- Emphasis was placed on modular design and readability.
- Proper memory management is handled throughout.
- Each task builds incrementally on the previous ones, culminating in compression and physics-like behaviors.
