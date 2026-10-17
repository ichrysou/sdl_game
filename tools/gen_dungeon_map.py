#!/usr/bin/env python3
"""Generate a dungeon tilemap XML for sdl_game from a rect-based layout.

Tile indices (assets/tiles2.png, 4 columns x 32px tiles):
  0 = stone floor
  1 = brick wall
  2 = water
  3 = grass

Layer 0 gets tile areas, layer 1 gets collision areas. Walls and water both
block movement, so the generator derives layer 1 from the same cells it
renders in layer 0 -- the two layers can never drift apart.

Usage: python3 tools/gen_dungeon_map.py > assets/dungeon_crypt.xml
"""

import sys
from collections import defaultdict

WIDTH = 64
HEIGHT = 56
TILEWIDTH = 32
TILEHEIGHT = 32
NAME = "Forgotten Crypt"

FLOOR = "8"   # cracked stone (new tile index 8)
WALL = "1"
WATER = "2"

# Rooms that keep plain stone (index 0) instead of cracked stone, for variety.
STONE_ROOMS = {0, 3, 5}  # entry hall, ossuary, treasure vault

# Individual tile decorations: (x, y, index, blocks_movement)
DECORATIONS = [
    # --- brick wall corners at room outer corners ---
    (2, 2, "5", True), (14, 2, "5", True),      # A entry hall top corners
    (22, 2, "5", True), (32, 2, "5", True),      # B armory top corners
    (36, 2, "5", True), (54, 2, "5", True),      # C grand hall top corners
    (2, 15, "5", True), (14, 15, "5", True),     # D ossuary
    (17, 14, "5", True), (29, 14, "5", True),    # E pool room
    (33, 15, "5", True), (45, 15, "5", True),    # F treasure vault
    (4, 26, "5", True), (59, 26, "5", True),     # G gallery ends
    (5, 32, "5", True), (17, 32, "5", True),     # H ritual chamber
    (21, 31, "5", True), (37, 31, "5", True),    # I prison
    (41, 32, "5", True), (54, 32, "5", True),    # J sanctum
    (23, 46, "5", True), (45, 46, "5", True),    # K boss room top
    # --- torches along the gallery walls (decoration only, never blocking) ---
    (10, 27, "16", False), (18, 27, "16", False), (26, 27, "16", False),
    (34, 27, "16", False), (42, 27, "16", False), (50, 27, "16", False),
    (10, 28, "16", False), (26, 28, "16", False), (50, 28, "16", False),
    # --- blood altar in boss room ---
    (34, 48, "21", True),
    # --- pit hazard in prison corridor ---
    (28, 38, "22", True),
    # --- necromancer rune in sanctum ---
    (48, 36, "23", False),
]

# (x, y, width, height) in tile coordinates
ROOMS = [
    (3, 3, 16, 9),    # A: entry hall  -> stone
    (23, 2, 10, 8),   # B: armory      -> cracked
    (37, 2, 18, 10),  # C: grand hall  -> cracked
    (3, 15, 12, 8),   # D: ossuary     -> stone
    (18, 14, 12, 10), # E: pool room   -> cracked
    (34, 15, 12, 9),  # F: treasure vault -> stone
    (4, 26, 56, 3),   # G: torch gallery (long corridor) -> cracked
    (6, 32, 12, 9),   # H: ritual chamber -> cracked
    (22, 31, 16, 10), # I: prison cells -> cracked
    (42, 32, 13, 9),  # J: sanctum -> cracked
    (24, 46, 22, 8),  # K: boss room -> cracked
]

CORRIDORS = [
    (19, 5, 4, 2),    # A - B
    (33, 5, 4, 2),    # B - C
    (9, 12, 3, 3),    # A - D
    (25, 10, 3, 4),   # B - E
    (44, 12, 3, 3),   # C - F
    (15, 18, 3, 2),   # D - E
    (30, 18, 4, 2),   # E - F
    (7, 23, 3, 4),    # D - G
    (23, 24, 3, 3),   # E - G
    (40, 24, 3, 3),   # F - G
    (52, 12, 3, 14),  # C - G (long eastern loop, aligned under the grand hall)
    (11, 30, 3, 2),   # G - H
    (28, 29, 3, 3),   # G - I
    (49, 30, 3, 2),   # G - J
    (38, 35, 4, 2),   # I - J
    (31, 41, 3, 5),   # I - K (boss room)
    (42, 41, 3, 5),   # J - K (boss room)
]

# Wall stubs carved back in after carving floors: pillars and cell dividers.
WALLS = [
    (12, 27, 1, 1),   # gallery pillars (staggered so they never block passage)
    (22, 28, 1, 1),
    (33, 27, 1, 1),
    (45, 28, 1, 1),
    (56, 28, 1, 1),
    (41, 4, 2, 2),    # grand hall pillars
    (50, 8, 2, 2),
    (37, 16, 1, 4),   # treasure vault inner walls
    (42, 19, 1, 4),
    (24, 31, 1, 2),   # prison cell dividers (clear of the G-I corridor)
    (27, 31, 1, 3),
    (32, 31, 1, 2),
    (35, 38, 1, 3),
]

WATER_POOLS = [
    (21, 17, 4, 4),   # pool room
    (10, 34, 4, 3),   # ritual chamber
    (46, 34, 4, 3),   # sanctum
]


def in_bounds(x, y, w, h):
    return 0 <= x and 0 <= y and x + w <= WIDTH and y + h <= HEIGHT


def fill(grid, x, y, w, h, value):
    assert in_bounds(x, y, w, h), f"rect out of bounds: {x}, {y}, {w}, {h}"
    for row in range(y, y + h):
        for col in range(x, x + w):
            grid[row][col] = value


def build_grid():
    grid = [[WALL] * WIDTH for _ in range(HEIGHT)]
    for i, rect in enumerate(ROOMS + CORRIDORS):
        tile = "0" if i in STONE_ROOMS else FLOOR
        fill(grid, *rect, tile)
    for rect in WALLS:
        fill(grid, *rect, WALL)
    for rect in WATER_POOLS:
        fill(grid, *rect, WATER)
    # decorations are NOT written into the grid -- they are emitted as
    # individual <tile> elements drawn on top of the merged areas.
    return grid


def blocking_indices():
    """Return the set of tile indices that block movement (colliders)."""
    return {WALL, WATER} | {index for _, _, index, blocks in DECORATIONS if blocks}


def merge_rects(cells_per_index):
    """Greedy rectangle merge: emit large rects from cell maps per index."""
    rects = defaultdict(list)
    for index, cells in cells_per_index.items():
        used = set()
        for y in range(HEIGHT):
            for x in range(WIDTH):
                if not cells[y][x] or (x, y) in used:
                    continue
                width = 1
                while (x + width < WIDTH and cells[y][x + width]
                       and (x + width, y) not in used):
                    width += 1
                height = 1
                while (y + height < HEIGHT
                       and all(cells[y + height][x + i] and (x + i, y + height) not in used
                               for i in range(width))):
                    height += 1
                for dy in range(height):
                    for dx in range(width):
                        used.add((x + dx, y + dy))
                rects[index].append((x, y, width, height))
    return rects


def emit(rects, decorations):
    lines = [
        f'<tilemap name="{NAME}" tileswide="{WIDTH}" tileshigh="{HEIGHT}" '
        f'tilewidth="{TILEWIDTH}" tileheight="{TILEHEIGHT}">',
        '  <layer number="0" name="Ground">',
    ]
    for index in sorted(rects):
        for x, y, w, h in rects[index]:
            lines.append(f'    <area x="{x}" y="{y}" width="{w}" height="{h}" index="{index}"/>')
    # individual decoration tiles (single cells that didn't merge into areas)
    for x, y, index, _blocks in decorations:
        lines.append(f'    <tile x="{x}" y="{y}" index="{index}" rot="0"/>')
    lines.append('  </layer>')
    lines.append('  <layer number="1" name="Collision">')
    # Merge all blocking cells (walls, water, blocking decorations) into rects.
    block = [[grid[y][x] in {WALL, WATER} for x in range(WIDTH)] for y in range(HEIGHT)]
    merged = merge_rects({"collider": block})
    for x, y, w, h in merged["collider"]:
        lines.append(f'    <area x="{x}" y="{y}" width="{w}" height="{h}"/>')
    # individual blocking decorations (1x1 colliders each)
    for x, y, index, blocks in decorations:
        if blocks:
            lines.append(f'    <area x="{x}" y="{y}" width="1" height="1"/>')
    lines.append('  </layer>')
    lines.append('</tilemap>')
    return lines


grid = build_grid()
# build per-index cell maps for the ground layer
unique_indices = sorted({grid[y][x] for y in range(HEIGHT) for x in range(WIDTH)})
cells_per_index = {}
for index in unique_indices:
    cells_per_index[index] = [[grid[y][x] == index for x in range(WIDTH)] for y in range(HEIGHT)]

sys.stdout.write("\n".join(emit(merge_rects(cells_per_index), DECORATIONS)) + "\n")
