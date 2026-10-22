#!/usr/bin/env python3
"""Generate assets/tiles2.png - Dark dungeon tilesheet inspired by dungeon.jpg.

Palette (cooled down from the reference image):
- Near-black purple-gray base (25, 26, 28)
- Cool blue-gray stone (57, 71, 87)
- Deep blue accents (15, 61, 139)
- Warm tan stone highlights (98, 88, 74)
- Moss green (70, 115, 71)

Keeps indices 0-3 so existing maps still load:
  0 = stone floor, 1 = brick wall, 2 = water, 3 = grass

Art goals:
- Dark, moody, unified color scheme (no warm reddish paint look)
- Cool blue-gray stone that blends when tiled
- Soft per-tile shading for depth without flat fills
- Regular stone-brick mortar grid (not random)
- Wall capstones with lighter top edge
- NO thick borders around tiles
- Each tile internally varied so large areas aren't repetitive
"""

from PIL import Image
import random

S = 32
COLS = 4

# 16-color master palette snapped from dungeon.jpg analysis.
PAL16 = [
    (12, 10, 16),    # 0 pit / near-black
    (25, 26, 28),    # 1 stone dark / near-black purple-gray (dominant)
    (57, 71, 87),    # 2 stone mid / cool blue-gray
    (86, 81, 73),    # 3 stone light / warm tan gray
    (130, 128, 132), # 4 stone highlight
    (56, 69, 85),    # 5 mortar / cool gray-blue
    (65, 62, 61),    # 6 wood dark / dark gray
    (98, 88, 74),    # 7 wood mid / warm tan stone
    (110, 105, 95),  # 8 wood light
    (91, 88, 82),    # 9 capstone
    (15, 61, 139),   # 10 water dark / deep blue
    (17, 60, 131),   # 11 water mid
    (57, 71, 87),    # 12 water light
    (24, 24, 27),    # 13 grass dark
    (70, 115, 71),   # 14 grass mid / moss green
    (24, 20, 26),    # 15 soft outline
]

PAL = {
    # stone (cool blue-gray, matches dungeon.jpg)
    "stoneD": (25, 26, 28),
    "stone": (57, 71, 87),
    "stoneL": (86, 81, 73),
    "stoneXL": (130, 128, 132),
    "stoneWear": (56, 69, 85),
    # mortar
    "mortar": (56, 69, 85),
    # wall capstones
    "capStone": (91, 88, 82),
    "capStoneL": (130, 128, 132),
    # water (deep blue from reference)
    "waterD": (15, 61, 139),
    "water": (17, 60, 131),
    "waterL": (57, 71, 87),
    # grass (moss green from reference)
    "grassD": (24, 24, 27),
    "grass": (70, 115, 71),
    "grassL": (90, 140, 90),
    # wood / decor (cool gray-tones)
    "woodD": (65, 62, 61),
    "wood": (98, 88, 74),
    "woodL": (110, 105, 95),
    "dirt": (86, 81, 73),
    "dirtL": (110, 105, 95),
    "iron": (67, 71, 77),
    "ironL": (100, 105, 110),
    "gold": (170, 150, 110),
    "bone": (200, 200, 200),
    "boneD": (150, 150, 150),
    "pit": (12, 10, 16),
    "pitEdge": (38, 42, 46),
    "torch": (220, 160, 90),
    "torchL": (240, 200, 150),
    "red": (150, 60, 60),
    "redD": (100, 40, 40),
    "purple": (130, 90, 160),
    "purpleL": (170, 130, 200),
    "teal": (90, 180, 160),
    "tealL": (140, 220, 200),
    "o": (24, 20, 26),
    "x": (15, 61, 139),
}


def nearest16(r, g, b):
    """Snap an RGB color to the nearest color in the 16-color palette."""
    best = PAL16[0]
    best_dist = (r - best[0]) ** 2 + (g - best[1]) ** 2 + (b - best[2]) ** 2
    for cand in PAL16[1:]:
        d = (r - cand[0]) ** 2 + (g - cand[1]) ** 2 + (b - cand[2]) ** 2
        if d < best_dist:
            best_dist = d
            best = cand
    return best


def new_img():
    return Image.new("RGBA", (COLS * S, 6 * S), (0, 0, 0, 0))


def px(img, idx, x, y, ch):
    if ch is None:
        return
    col = idx % COLS
    row = idx // COLS
    gx = col * S + x
    gy = row * S + y
    if 0 <= gx < img.width and 0 <= gy < img.height:
        r, g, b = PAL[ch]
        r, g, b = nearest16(r, g, b)
        img.putpixel((gx, gy), (r, g, b, 255))


def rect(img, idx, x0, y0, x1, y1, ch):
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            px(img, idx, x, y, ch)


def hline(img, idx, x0, x1, y, ch):
    for x in range(min(x0, x1), max(x0, x1) + 1):
        px(img, idx, x, y, ch)


def vline(img, idx, x, y0, y1, ch):
    for y in range(min(y0, y1), max(y0, y1) + 1):
        px(img, idx, x, y, ch)


def rect_outline(img, idx, x0, y0, x1, y1, ch):
    for x in range(x0, x1 + 1):
        px(img, idx, x, y0, ch)
        px(img, idx, x, y1, ch)
    for y in range(y0, y1 + 1):
        px(img, idx, x0, y, ch)
        px(img, idx, x1, y, ch)


def noise(img, idx, x0, y0, x1, y1, base, light, dark, seed=0, p_light=0.10, p_dark=0.07):
    rnd = random.Random(seed)
    for y in range(y0, y1 + 1):
        for x in range(x0, x1 + 1):
            r = rnd.random()
            if r < p_light:
                px(img, idx, x, y, light)
            elif r < p_light + p_dark:
                px(img, idx, x, y, dark)
            else:
                px(img, idx, x, y, base)


def wear_patches(img, idx, x0, y0, x1, y1, wear_ch, seed=0, density=0.09, size=2):
    rnd = random.Random(seed)
    area = (x1 - x0 + 1) * (y1 - y0 + 1)
    n = int(area * density)
    for _ in range(n):
        cx = rnd.randint(x0, x1)
        cy = rnd.randint(y0, y1)
        r = rnd.randint(1, size)
        rect(img, idx, cx - r, cy - r, cx + r, cy + r, wear_ch)


def brick_wall(img, idx, x0, y0, x1, y1, seed=0):
    """Regular stone-brick grid with thin, consistent mortar (not random)."""
    bh = 6
    row_y = y0
    while row_y <= y1:
        x = x0 + ((row_y // bh) % 2) * bh // 2
        while x <= x1:
            bx1 = min(x + bh - 1, x1)
            by1 = min(row_y + bh - 1, y1)
            shade = "stone" if (row_y // bh + x // bh) % 2 == 0 else "stoneL"
            rect(img, idx, x, row_y, bx1, by1, shade)
            x += bh
        row_y += bh
    # Consistent thin mortar lines on every row/column boundary
    ry = y0
    while ry <= y1:
        hline(img, idx, x0, x1, ry, "mortar")
        ry += bh
    cx = x0
    while cx <= x1:
        vline(img, idx, cx, y0, y1, "mortar")
        cx += bh


def wall_cap(img, idx, base_ch, cap_ch, capL_ch, seed=0):
    """Stone wall: lighter capstone row at top, subtle side highlights."""
    rnd = random.Random(seed)
    rect(img, idx, 0, 0, S - 1, S - 1, base_ch)
    noise(img, idx, 0, 0, S - 1, S - 1, base_ch, "stoneL", "stoneD", seed=seed)
    for x in range(S):
        px(img, idx, x, 0, cap_ch)
        px(img, idx, x, 1, capL_ch if rnd.random() < 0.4 else base_ch)
    for y in range(2, S):
        if rnd.random() < 0.25:
            px(img, idx, 0, y, cap_ch)
            px(img, idx, S - 1, y, cap_ch)


def apply_soft_shading(img, idx, seed=0):
    """Soft top-left light source per tile for depth."""
    rnd = random.Random(seed)
    col = idx % COLS
    row = idx // COLS
    ox = col * S
    oy = row * S
    for y in range(S):
        for x in range(S):
            r, g, b, a = img.getpixel((ox + x, oy + y))
            if a == 0:
                continue
            light = 1.0 - (x * 0.009 + y * 0.011) + rnd.uniform(-0.02, 0.02)
            light = max(0.60, min(1.08, light))
            img.putpixel((ox + x, oy + y),
                         (min(255, int(r * light)),
                          min(255, int(g * light)),
                          min(255, int(b * light)), a))


def build():
    img = new_img()

    # ---------- row 0: indices 0-5 ----------
    # 0 stone floor — dark near-black base with subtle grain, NO grid cracks
    rect(img, 0, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 0, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=0,
          p_light=0.06, p_dark=0.05)
    wear_patches(img, 0, 0, 0, S - 1, S - 1, "stoneWear", seed=100, density=0.08, size=2)

    # 1 brick wall — cool stone brick, regular mortar, capstone top
    rect(img, 1, 0, 0, S - 1, S - 1, "stoneD")
    brick_wall(img, 1, 0, 0, S - 1, S - 1, seed=1)
    for x in range(S):
        px(img, 1, x, 0, "capStone")
        px(img, 1, x, 1, "capStoneL")
    for y in range(2, S):
        px(img, 1, 0, y, "capStone")
        px(img, 1, S - 1, y, "capStone")

    # 2 water — deep blue
    rect(img, 2, 0, 0, S - 1, S - 1, "waterD")
    noise(img, 2, 0, 0, S - 1, S - 1, "waterD", "waterL", "waterD", seed=2,
          p_light=0.08, p_dark=0.06)
    for yy in range(4, S - 1, 8):
        hline(img, 2, 0, S - 1, yy, "waterL")
    for yy in range(7, S - 1, 15):
        hline(img, 2, 2, S - 3, yy, "waterL")

    # 3 grass — moss green
    rect(img, 3, 0, 0, S - 1, S - 1, "grassD")
    noise(img, 3, 0, 0, S - 1, S - 1, "grassD", "grassL", "grassD", seed=3,
          p_light=0.10, p_dark=0.06)
    for yy in range(5, S - 1, 12):
        for xx in range(3, S - 1, 11):
            px(img, 3, xx, yy, "grassL")

    # 4 stone wall with capstone (variant of 1)
    rect(img, 4, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 4, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=4,
          p_light=0.08, p_dark=0.06)
    wall_cap(img, 4, "stoneD", "capStone", "capStoneL", seed=4)

    # 5 brick wall variant — slightly lighter stone
    rect(img, 5, 0, 0, S - 1, S - 1, "stoneD")
    brick_wall(img, 5, 0, 0, S - 1, S - 1, seed=5)
    for x in range(S):
        px(img, 5, x, 0, "capStone")
        px(img, 5, x, 1, "capStoneL")
    for y in range(2, S):
        px(img, 5, 0, y, "capStone")
        px(img, 5, S - 1, y, "capStone")

    # ---------- row 1: indices 6-11 ----------
    # 6 dungeon door — cool wood
    rect(img, 6, 2, 2, S - 3, S - 3, "woodD")
    hline(img, 6, 4, S - 4, 6, "iron")
    hline(img, 6, 4, S - 4, S - 5, "iron")
    vline(img, 6, 8, 2, S - 2, "iron")
    vline(img, 6, S - 6, 2, S - 2, "iron")
    rect_outline(img, 6, S - 10, 10, S - 6, 14, "iron")
    px(img, 6, S - 9, 11, "gold")
    px(img, 6, S - 7, 11, "gold")

    # 7 stairs down
    rect(img, 7, 0, 0, S - 1, S - 1, "stoneD")
    for step in range(5):
        x0 = step * 6
        x1 = min(x0 + 5, S - 1)
        y0 = S - 1 - step * 5
        rect(img, 7, x0, y0, x1, S - 1, "stoneL")
        rect(img, 7, x0, y0 - 1, x1, y0, "stoneXL")

    # 8 worn stone floor variant — more wear
    rect(img, 8, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 8, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=8,
          p_light=0.08, p_dark=0.08)
    wear_patches(img, 8, 0, 0, S - 1, S - 1, "stoneWear", seed=200, density=0.14, size=3)

    # 9 blood splatter on stone
    rect(img, 9, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 9, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=9,
          p_light=0.06, p_dark=0.05)
    rnd = random.Random(99)
    for _ in range(40):
        x = rnd.randint(3, S - 4); y = rnd.randint(3, S - 4)
        r = rnd.randint(1, 3)
        rect(img, 9, x - r, y - r, x + r, y + r, "red")
        if rnd.random() < 0.4:
            px(img, 9, x, y, "redD")

    # 10 dirt path — warm gray-brown
    rect(img, 10, 0, 0, S - 1, S - 1, "dirt")
    noise(img, 10, 0, 0, S - 1, S - 1, "dirt", "dirtL", "stoneD", seed=10,
          p_light=0.08, p_dark=0.06)

    # 11 carpet / rug — soft outline only
    rect(img, 11, 2, 2, S - 3, S - 3, "stoneD")
    noise(img, 11, 2, 2, S - 3, S - 3, "stoneD", "stoneL", "redD", seed=11,
          p_light=0.08, p_dark=0.06)
    rect_outline(img, 11, 2, 2, S - 3, S - 3, "o")
    hline(img, 11, 4, S - 4, 6, "o")
    hline(img, 11, 4, S - 4, S - 5, "o")
    vline(img, 11, 8, 2, S - 2, "o")
    vline(img, 11, S - 6, 2, S - 2, "o")

    # ---------- row 2: indices 12-15 ----------
    # 12 skull pile
    rect(img, 12, 8, 22, S - 9, S - 1, "bone")
    for sy in range(22, S - 1, 6):
        for sx in range(8, S - 8, 7):
            px(img, 12, sx, sy, "boneD")
    px(img, 12, 11, 25, "o"); px(img, 12, 14, 25, "o")
    px(img, 12, 12, 26, "x")
    px(img, 12, 19, 25, "o"); px(img, 12, 22, 25, "o")
    px(img, 12, 20, 26, "x")

    # 13 treasure chest (closed) — cool wood
    rect(img, 13, 4, 16, S - 5, S - 2, "woodD")
    rect_outline(img, 13, 3, 14, S - 4, 18, "wood")
    hline(img, 13, 6, S - 6, 18, "iron")
    hline(img, 13, 6, S - 6, 17, "ironL")
    px(img, 13, 15, 19, "gold"); px(img, 13, 16, 19, "gold")
    px(img, 13, 15, 20, "o"); px(img, 13, 16, 20, "o")
    px(img, 13, S - 4, 20, "gold"); px(img, 13, S - 3, 20, "gold")

    # 14 stone floor variant C — lighter grain, sparse wear
    rect(img, 14, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 14, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=14,
          p_light=0.07, p_dark=0.05)
    wear_patches(img, 14, 0, 0, S - 1, S - 1, "stoneWear", seed=300, density=0.06, size=2)

    # 15 stone floor variant D — darker, subtle cracks
    rect(img, 15, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 15, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=15,
          p_light=0.05, p_dark=0.08)
    wear_patches(img, 15, 0, 0, S - 1, S - 1, "stoneWear", seed=400, density=0.16, size=3)
    hline(img, 15, 2, S - 3, 8, "stoneWear")
    hline(img, 15, 4, S - 4, 20, "stoneWear")
    vline(img, 15, 8, 2, S - 3, "stoneWear")
    vline(img, 15, 22, 4, S - 4, "stoneWear")

    # ---------- row 3: indices 16-23 ----------
    # 16 torch sconce — small, non-blocking decoration
    rect(img, 16, 24, 12, S - 1, 20, "stoneD")
    hline(img, 16, 24, 26, 10, "iron"); hline(img, 16, 24, 26, 11, "ironL")
    vline(img, 16, 25, 10, 20, "iron")
    hline(img, 16, 25, 27, 6, "torch")
    hline(img, 16, 25, 27, 7, "torch")
    hline(img, 16, 25, 26, 8, "torchL")
    px(img, 16, 26, 5, "torchL")
    px(img, 16, 26, 9, "torch")

    # 17 water shore — stone meeting water
    rect(img, 17, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 17, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=17,
          p_light=0.06, p_dark=0.05)
    for y in range(14, S):
        for x in range(S - y + 12, S):
            if x < S:
                px(img, 17, x, y, "waterD")

    # 18 grass-to-dirt transition
    rect(img, 18, 0, 0, S - 1, S - 1, "grassD")
    noise(img, 18, 0, 0, S - 1, S - 1, "grassD", "grassL", "grassD", seed=18,
          p_light=0.10, p_dark=0.06)
    rect(img, 18, 16, 16, S - 1, S - 1, "dirt")
    noise(img, 18, 16, 16, S - 1, S - 1, "dirt", "dirtL", "stoneD", seed=118,
          p_light=0.08, p_dark=0.06)
    for yy in range(14, 18):
        hline(img, 18, 14, S - 1, yy, "dirt")

    # 19 iron bars
    rect(img, 19, 0, 0, S - 1, S - 1, "stoneD")
    for bx in (10, 16, 22):
        vline(img, 19, bx, 2, S - 2, "iron")
        px(img, 19, bx, 0, "ironL"); px(img, 19, bx, S - 1, "ironL")

    # 20 magic circle (teal runes)
    rect(img, 20, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 20, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=20,
          p_light=0.06, p_dark=0.05)
    hline(img, 20, 6, S - 7, 4, "teal"); hline(img, 20, 6, S - 7, S - 5, "teal")
    vline(img, 20, 4, 6, S - 7, "teal"); vline(img, 20, S - 5, 6, S - 7, "teal")
    px(img, 20, 16, 16, "teal"); px(img, 20, 16, 17, "tealL")
    px(img, 20, 15, 15, "teal"); px(img, 20, 17, 15, "teal")
    px(img, 20, 15, 17, "teal"); px(img, 20, 17, 17, "teal")

    # 21 blood altar / table
    rect(img, 21, 4, 4, S - 5, 10, "stoneD")
    rect(img, 21, 8, 10, S - 9, S - 1, "woodD")
    rnd = random.Random(21)
    for _ in range(18):
        x = rnd.randint(10, S - 11); y = rnd.randint(12, S - 2)
        px(img, 21, x, y, "red")

    # 22 pit / hole — dark, subtle rim
    rect(img, 22, 0, 0, S - 1, S - 1, "stoneD")
    rect(img, 22, 8, 8, S - 9, S - 9, "pit")
    rect_outline(img, 22, 10, 10, S - 11, S - 11, "pitEdge")
    px(img, 22, 16, 16, "pit")

    # 23 necromancer rune mark (purple)
    rect(img, 23, 0, 0, S - 1, S - 1, "stoneD")
    noise(img, 23, 0, 0, S - 1, S - 1, "stoneD", "stoneL", "stoneWear", seed=23,
          p_light=0.06, p_dark=0.05)
    hline(img, 23, 8, S - 9, 10, "purple"); hline(img, 23, 8, S - 9, S - 11, "purple")
    vline(img, 23, 10, 8, S - 9, "purple"); vline(img, 23, S - 11, 8, S - 9, "purple")
    px(img, 23, 16, 16, "purple"); px(img, 23, 15, 15, "purple")
    px(img, 23, 17, 15, "purple"); px(img, 23, 15, 17, "purple")
    px(img, 23, 17, 17, "purple")

    # soft top-left shading on every tile
    for idx in range(COLS * 6):
        apply_soft_shading(img, idx, seed=1000 + idx)

    img.save("assets/tiles2.png")
    print("wrote assets/tiles2.png", img.size)


if __name__ == "__main__":
    build()
