#!/usr/bin/env python3
"""Generate assets/necromancer.png - a 32x32-frame pixel-art sheet.

Sheet layout (4 columns x 4 rows, each cell 32x32):
  row 0: idle frame 0, idle frame 1
  row 1: walk frames 0..3
  row 2: die frames 0..3
  row 3: revive frames 0..4 placeholder (future "raise skeletons" skill)

Palette intent: dark violet robes, platinum hair, pale skin, glowing
teal eyes/orb. Outlined in near-black purple for readability on any tile.
"""

from PIL import Image

S = 32
COLS, ROWS = 4, 4

PAL = {
    "o": (26, 16, 38),     # outline
    "c": (40, 24, 62),     # cape shadow
    "R": (58, 34, 92),     # robe dark
    "r": (88, 52, 140),    # robe mid
    "l": (118, 84, 180),   # robe light
    "T": (222, 180, 90),   # belt/trim
    "H": (216, 223, 240),  # platinum hair
    "s": (232, 213, 192),  # skin
    "e": (125, 252, 209),  # eye/orb glow
    "w": (107, 74, 45),    # staff wood
    "W": (140, 100, 62),   # staff wood light
    "b": (238, 238, 238),  # bone
    "x": (18, 10, 28),     # deep shadow
}


def make_canvas():
    img = Image.new("RGBA", (COLS * S, ROWS * S), (0, 0, 0, 0))
    return img


def px(img, gx, gy, ch, frame=(0, 0)):
    """Draw one pixel (palette char) into frame (col, row) at local coords."""
    if ch is None:
        return
    col, row = frame
    x = col * S + gx
    y = row * S + gy
    if 0 <= x < img.width and 0 <= y < img.height:
        img.putpixel((x, y), (*PAL[ch], 255))


def hline(img, frame, x0, x1, y, ch):
    for x in range(min(x0, x1), max(x0, x1) + 1):
        px(img, x, y, ch, frame)


def vline(img, frame, x, y0, y1, ch):
    for y in range(min(y0, y1), max(y0, y1) + 1):
        px(img, x, y, ch, frame)


def draw_figure(img, frame, bob=0, hem_flare=0, step=0, sway=0, staff=("held", -1),
                orb_lit=True, kneel=None, dissolve=0.0, wisps=0, head_dx=0):
    """staff: (mode, dx) with mode 'held' | 'fallen' | 'ground' | None.
    kneel: None | 0 (crouching) | 1 (collapsed) | 2 (flat). dissolve: 0..1.
    head_dx: shift the head left/right (stagger / lean)."""
    # ---------- ground shadow ----------
    if kneel is None:
        for x in range(10, 23):
            px(img, x, 30, "x", frame)

    # ---------- cape (small wedge behind the left shoulder) ----------
    if kneel is None:
        for y in range(14 + bob, 21 + bob):
            offset = (y - (14 + bob)) // 4
            px(img, 9 - offset, y, "c", frame)
            px(img, 8 - offset, y, "c", frame)
            px(img, 9 - offset, 20 + bob, "c", frame) if y == 20 + bob else None

    # ---------- staff ----------
    sx = 26  # staff column, clear of the robe
    staff_mode, sdx = ("none", 0) if staff is None else staff

    if staff_mode == "held":
        top, bottom = 5 + bob, 28 + bob
        vline(img, frame, sx + sdx, top + 1, bottom, "w")
        vline(img, frame, sx + 1 + sdx, top + 2, bottom, "w")
        hline(img, frame, sx - 1 + sdx, sx + 2 + sdx, top, "w")
        # crystal orb
        oc = "e" if orb_lit else "R"
        hline(img, frame, sx - 1 + sdx, sx + 2 + sdx, top - 2, oc)
        hline(img, frame, sx - 1 + sdx, sx + 2 + sdx, top - 1, oc)
        if orb_lit:
            px(img, sx + sdx, top - 3, "e", frame)
        # carved skull ring under the orb
        hline(img, frame, sx + sdx, sx + 2 + sdx, top + 1, "b")
        px(img, sx + 1 + sdx, top + 2, "b", frame)
    elif staff_mode == "fallen":
        # staff tumbling diagonally
        for i in range(6):
            px(img, 19 + i, 24 + (i + 1) // 2, "w", frame)
        px(img, 25, 23, "e" if orb_lit else "b", frame)
        px(img, 25, 24, "b", frame)
    elif staff_mode == "ground":
        # staff lying flat in front of the body
        hline(img, frame, 18, 26, 29, "w")
        hline(img, frame, 19, 26, 28, "x")
        px(img, 18, 28, "b", frame)
        px(img, 26, 28, "e" if orb_lit else "b", frame)

    # ---------- body / robe ----------
    if kneel is None:
        body_top, body_bot = 12 + bob, 27 + bob
        for y in range(body_top, body_bot + 1):
            t = (y - body_top) / (body_bot - body_top)
            half = 4 + int(t * (6 + hem_flare))  # 4px shoulders -> ~10px hem
            dx = int(round(sway * t))            # skirt sweeps with the step
            for x in range(16 - half, 16 + half + 1):
                edge = x in (16 - half, 16 + half)
                px(img, x + dx, y, "o" if edge else ("R" if (x * 3 + y) % 5 == 0 else "r"), frame)
        # fold highlights
        for y in range(body_top, body_bot + 1):
            px(img, 14, y, "l", frame)
            px(img, 18, y, "l" if (y // 4) % 2 == 0 else "r", frame)
        # belt
        bh = 4 + 6 // 2
        hline(img, frame, 16 - bh, 16 + bh, body_top + 5, "T")
        px(img, 16, body_top + 5, "o", frame)
        # feet peeking under the hem
        fy = body_bot + 1
        left_foot = 12 + step
        right_foot = 19 - step
        hline(img, frame, left_foot, left_foot + 2, fy, "x")
        hline(img, frame, right_foot, right_foot + 2, fy, "x")
        # sleeves: hands
        px(img, 11, body_top + 4, "s", frame)
        px(img, 11, body_top + 5, "o", frame)
    else:
        levels = [
            [(14, 26), (13, 27)],     # crouching
            [(12, 28), (7, 28)],      # collapsed mound
            [(9, 29), (6, 30)],       # flat puddle
        ]
        l1, l2 = levels[min(kneel, 2)]
        top, bot = l1
        for y in range(top, bot + 1):
            t = (y - top) / max(1, bot - top)
            half = 5 + int(t * 4)
            for x in range(16 - half, 16 + half + 1):
                edge = x in (16 - half, 16 + half)
                px(img, x, y, "o" if edge else ("R" if (x * 3 + y) % 5 == 0 else "r"), frame)
        if kneel >= 1:
            top, bot = l2
            for y in range(top, bot + 1):
                t = (y - top) / max(1, bot - top)
                half = 4 + int(t * 3)
                for x in range(16 - half, 16 + half + 1):
                    edge = x in (16 - half, 16 + half)
                    px(img, x, y, "o" if edge else "R", frame)

    # ---------- hooded head ----------
    hx = head_dx
    if kneel is None:
        hy = 2 + bob
        # hood silhouette: peak then widening
        px(img, 15 + hx, hy, "o", frame)
        px(img, 16 + hx, hy, "o", frame)
        for y in range(hy + 1, hy + 6):
            half = 2 + (y - hy - 1)      # 2..6
            for x in range(16 - half, 16 + half):
                edge = x in (16 - half, 15 + half)
                ch = "o" if edge else ("R" if (x + y) % 4 else "r")
                px(img, x + hx, y, ch, frame)
        # hood interior shadow
        hline(img, frame, 14 + hx, 19 + hx, hy + 4, "x")
        # pale face, small and high inside the hood
        hline(img, frame, 15 + hx, 19 + hx, hy + 5, "s")
        px(img, 16 + hx, hy + 6, "s", frame)
        px(img, 15 + hx, hy + 5, "e" if orb_lit else "x", frame)
        px(img, 18 + hx, hy + 5, "e" if orb_lit else "x", frame)
        px(img, 17 + hx, hy + 6, "x", frame)
        # hood rim below the face
        hline(img, frame, 13 + hx, 20 + hx, hy + 7, "R")
        hline(img, frame, 13 + hx, 20 + hx, hy + 8, "o")
        # long hair framing the face only below the hood rim
        for y in range(hy + 6, hy + 11):
            px(img, 12 + hx, y, "H", frame)
            px(img, 21 + hx, y, "H", frame)
        px(img, 11 + hx, hy + 10, "H", frame)
        px(img, 22 + hx, hy + 10, "H", frame)
        px(img, 12 + hx, hy + 11, "H", frame)
        px(img, 21 + hx, hy + 11, "H", frame)
    else:
        my = 24 if kneel == 0 else 26
        hline(img, frame, 13 + hx, 19 + hx, my, "o")
        hline(img, frame, 13 + hx, 19 + hx, my + 1, "R")
        px(img, 14 + hx, my + 1, "H", frame)
        px(img, 15 + hx, my, "e" if orb_lit else "x", frame)

    # ---------- dissolve away robe/skin pixels ----------
    if dissolve > 0:
        dy = int(dissolve * 12)
        for y in range(29, 29 - dy, -1):
            for x in range(6, 27):
                if (x * 7 + y * 3) % 4 < 2:
                    img.putpixel((frame[0] * S + x, frame[1] * S + y), (0, 0, 0, 0))

    # ---------- rising wisps ----------
    if wisps > 0:
        for i in range(wisps):
            wx = 11 + (i * 4) % 12
            wy = 25 - i * 3
            if wy >= 0:
                px(img, wx, wy, "e", frame)


def build():
    img = make_canvas()

    # ---- row 0: idle (2 frames, orb pulse) ----
    draw_figure(img, (0, 0), staff=("held", 0))
    draw_figure(img, (1, 0), bob=1, staff=("held", 1))

    # ---- row 1: walk (4 frames) ----
    draw_figure(img, (0, 1), bob=2, step=-2, sway=-2, staff=("held", 0), head_dx=-1)   # down-step, lean back
    draw_figure(img, (1, 1), bob=-1, hem_flare=1, staff=("held", 1), head_dx=0)         # up-step, flare
    draw_figure(img, (2, 1), bob=2, step=2, sway=2, staff=("held", 0), head_dx=1)       # down-step, lean forward
    draw_figure(img, (3, 1), bob=-1, hem_flare=1, staff=("held", 1), head_dx=0)         # up-step, flare

    # ---- row 2: die (4 frames) ----
    draw_figure(img, (0, 2), staff=("fallen", 0), orb_lit=False, wisps=2, head_dx=1)    # stagger, orb dying
    draw_figure(img, (1, 2), kneel=0, staff=("ground", 0), orb_lit=False, wisps=3, head_dx=0)  # crouch, head down
    draw_figure(img, (2, 2), kneel=1, staff=("ground", 0), orb_lit=False, wisps=5, head_dx=-1) # collapsed flat
    draw_figure(img, (3, 2), kneel=2, dissolve=0.6, staff=None, orb_lit=False, wisps=10, head_dx=0)  # dissolving

    # ---- row 3: revive placeholder (4 frames) ----
    draw_figure(img, (0, 3), kneel=2, staff=None, wisps=10)
    draw_figure(img, (1, 3), kneel=1, staff=("ground", 0), orb_lit=True, wisps=6)
    draw_figure(img, (2, 3), kneel=0, staff=("ground", 0), orb_lit=True, wisps=3)
    draw_figure(img, (3, 3), staff=("held", 0), orb_lit=True, wisps=1)

    img.save("assets/necromancer.png")
    print("wrote assets/necromancer.png", img.size)


if __name__ == "__main__":
    build()
