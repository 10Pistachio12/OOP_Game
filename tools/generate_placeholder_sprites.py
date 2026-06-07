#!/usr/bin/env python3
"""Generate transparent PNG placeholder sprites for early gameplay iteration."""

from __future__ import annotations

import struct
import zlib
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT_DIR = ROOT / "Resources" / "Sprites"
SCALE = 2

PALETTE = {
    ".": (0, 0, 0, 0),
    "B": (38, 91, 184, 255),
    "C": (59, 206, 225, 255),
    "D": (42, 35, 68, 255),
    "E": (244, 190, 74, 255),
    "F": (255, 214, 170, 255),
    "G": (70, 190, 103, 255),
    "H": (88, 48, 32, 255),
    "K": (22, 22, 28, 255),
    "M": (229, 80, 197, 255),
    "N": (139, 82, 49, 255),
    "O": (244, 125, 66, 255),
    "P": (122, 90, 196, 255),
    "R": (199, 67, 52, 255),
    "W": (255, 250, 220, 255),
    "X": (48, 52, 62, 255),
    "Y": (247, 217, 94, 255),
    "g": (39, 126, 72, 255),
}

SPRITES = {
    "player": [
        "................",
        "......BBBB......",
        ".....BCCCCB.....",
        ".....CFFFFC.....",
        ".....CFFFFC.....",
        "......CCCC......",
        ".....BBBBBB.....",
        "....BBYYYYBB....",
        "....BYYYYYYB....",
        "....BYYYYYYB....",
        ".....BBBBBB.....",
        ".....B....B.....",
        "....BB....BB....",
        "...BBB....BBB...",
        "................",
        "................",
    ],
    "player_marker": [
        ".......KK.......",
        ".......WW.......",
        "................",
        ".....K....K.....",
        "....K......K....",
        "...K...CC...K...",
        ".....C....C.....",
        "KW...C....C...WK",
        "KW...C....C...WK",
        ".....C....C.....",
        "...K...CC...K...",
        "....K......K....",
        ".....K....K.....",
        "................",
        ".......WW.......",
        ".......KK.......",
    ],
    "slime": [
        "................",
        "................",
        "......GGGG......",
        "....GGGGGGGG....",
        "...GGGGGGGGGG...",
        "..GGGGWGGWGGGG..",
        "..GGGGGGGGGGGG..",
        ".GGGGGGGGGGGGGG.",
        ".GGGGGGGGGGGGGG.",
        ".GGGGgGGGGgGGGG.",
        "..GGGggggggGGG..",
        "...GGGGGGGGGG...",
        "....GGGGGGGG....",
        "................",
        "................",
        "................",
    ],
    "bat": [
        "................",
        "................",
        "..DD........DD..",
        ".DDDD......DDDD.",
        "DDPPDD....DDPPDD",
        "DDPPDDDDDDDDPPDD",
        ".DDPPDDDDDDPPDD.",
        "..DDDDPDDPDDDD..",
        "....DDKDDKDD....",
        ".....DDDDDD.....",
        "......DDDD......",
        ".......DD.......",
        "................",
        "................",
        "................",
        "................",
    ],
    "brute": [
        "................",
        ".....RRRRRR.....",
        "....RRRRRRRR....",
        "...RRKRRRRKRR...",
        "...RRRRRRRRRR...",
        "....RRROORRR....",
        "...RRRRRRRRRR...",
        "..RRRRRRRRRRRR..",
        ".RRRROOOOOORRRR.",
        ".RRROOOOOOOORRR.",
        "..RRROOOOOORRR..",
        "...RRRRRRRRRR...",
        "...RRR....RRR...",
        "..RRR......RRR..",
        "................",
        "................",
    ],
    "elite": [
        "................",
        "....E..E..E.....",
        "...EEEEEEEEEE...",
        "...ERRRRRRRRE...",
        "..ERRKRRRRKRRE..",
        "..ERRRRRRRRRRE..",
        "...ERRROORRE....",
        "..ERRRRRRRRRRE..",
        ".EERRYYYYYYRREE.",
        ".EERRYYYYYYRREE.",
        "..EERRRRRRRREE..",
        "...EERRRRRREE...",
        "...ERR....RRE...",
        "..ERR......RRE..",
        "................",
        "................",
    ],
    "gem": [
        "................",
        "................",
        ".......W........",
        "......CCC.......",
        ".....CCCCC......",
        "....CCBCCCC.....",
        "...CCBBBBCCC....",
        "..CCBBBBBBCCC...",
        "...CCBBBBCCC....",
        "....CCBBCCC.....",
        ".....CCCCC......",
        "......CCC.......",
        ".......C........",
        "................",
        "................",
        "................",
    ],
    "chest": [
        "................",
        "................",
        "...NNNNNNNNNN...",
        "..NNYYYYYYYYNN..",
        "..NYYYYYYYYYYN..",
        "..NYYNNNNNNYYN..",
        "..NNNNNNNNNNNN..",
        "..NHHHHHHHHHHN..",
        "..NHHHHYYHHHHN..",
        "..NHHHHYYHHHHN..",
        "..NHHHHHHHHHHN..",
        "..NNNNNNNNNNNN..",
        "...NNNNNNNNNN...",
        "................",
        "................",
        "................",
    ],
    "magic_bolt": [
        "................",
        "................",
        ".......Y........",
        "......YYY.......",
        ".....YWWWY......",
        "....YWWWWWY.....",
        "...YYWWWWWYY....",
        "..YYWWWWWWWYY...",
        "...YYWWWWWYY....",
        "....YWWWWWY.....",
        ".....YWWWY......",
        "......YYY.......",
        ".......Y........",
        "................",
        "................",
        "................",
    ],
    "nova_bolt": [
        "................",
        ".......M........",
        ".......M........",
        "...M...P...M....",
        "....M..P..M.....",
        ".....MPPPM......",
        "..MMPPWCPPMM....",
        "...PPWWCWWPP....",
        "..MMPPWCPPMM....",
        ".....MPPPM......",
        "....M..P..M.....",
        "...M...P...M....",
        ".......M........",
        ".......M........",
        "................",
        "................",
    ],
    "shield_orb": [
        "................",
        "................",
        "......CCCC......",
        "....CCWWWWCC....",
        "...CWWCCCCWWC...",
        "...CWCCYYCCWC...",
        "..CWCCYYYYCCWC..",
        "..CWCCYYYYCCWC..",
        "...CWCCYYCCWC...",
        "...CWWCCCCWWC...",
        "....CCWWWWCC....",
        "......CCCC......",
        "................",
        "................",
        "................",
        "................",
    ],
}

HEALTH_BAR_STEPS = 10


def make_health_bar_pattern(step: int) -> list[str]:
    interior_width = 18
    fill_width = round(interior_width * step / HEALTH_BAR_STEPS)
    fill_color = "G" if step >= 6 else "Y" if step >= 3 else "R"
    fill = fill_color * fill_width + "X" * (interior_width - fill_width)

    return [
        "W" * 20,
        f"W{fill}W",
        f"W{fill}W",
        "W" * 20,
    ]


for health_step in range(HEALTH_BAR_STEPS + 1):
    SPRITES[f"health_bar_{health_step}"] = make_health_bar_pattern(health_step)


def make_chunk(kind: bytes, payload: bytes) -> bytes:
    checksum = zlib.crc32(kind + payload) & 0xFFFFFFFF
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", checksum)


def write_png(path: Path, pattern: list[str]) -> None:
    width = len(pattern[0])
    height = len(pattern)
    if any(len(row) != width for row in pattern):
        raise ValueError(f"{path.stem} has uneven row widths")

    scaled_width = width * SCALE
    scaled_height = height * SCALE
    raw = bytearray()

    for source_row in pattern:
        expanded_row = [PALETTE[pixel] for pixel in source_row for _ in range(SCALE)]
        row_bytes = b"".join(bytes(pixel) for pixel in expanded_row)
        for _ in range(SCALE):
            raw.append(0)
            raw.extend(row_bytes)

    header = struct.pack(">IIBBBBB", scaled_width, scaled_height, 8, 6, 0, 0, 0)
    png = (
        b"\x89PNG\r\n\x1a\n"
        + make_chunk(b"IHDR", header)
        + make_chunk(b"IDAT", zlib.compress(bytes(raw), level=9))
        + make_chunk(b"IEND", b"")
    )
    path.write_bytes(png)


def main() -> None:
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    for sprite_name, pattern in SPRITES.items():
        write_png(OUT_DIR / f"{sprite_name}.png", pattern)


if __name__ == "__main__":
    main()
