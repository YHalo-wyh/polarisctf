import re
from pathlib import Path

from PIL import Image, ImageDraw, ImageFont


root = Path(__file__).resolve().parent
src = root / "gdb_capture.txt"
dst = root / "pwndbg_keypoint.png"

text = src.read_text(encoding="utf-8", errors="ignore")
text = re.sub(r"\x1b\[[0-9;?]*[ -/]*[@-~]", "", text)

start = text.find("==== frame info ====")
if start != -1:
    text = text[start:]

lines = text.splitlines()
max_lines = 70
if len(lines) > max_lines:
    lines = lines[:max_lines]
    lines.append("... (truncated)")

title = "Real pwndbg session at a key exploit point"
lines = [title, ""] + lines

font = ImageFont.truetype(r"C:\Windows\Fonts\consola.ttf", 20)
line_h = 28
pad = 24
width = 1900
height = pad * 2 + line_h * len(lines)

img = Image.new("RGB", (width, height), (19, 22, 26))
draw = ImageDraw.Draw(img)

y = pad
for i, line in enumerate(lines):
    color = (220, 220, 220)
    if i == 0:
        color = (120, 220, 150)
    elif line.startswith("===="):
        color = (255, 203, 107)
    draw.text((pad, y), line, font=font, fill=color)
    y += line_h

img.save(dst)
print(dst)
