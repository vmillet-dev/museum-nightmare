import os
from PIL import Image, ImageDraw

# Create a 256x512 transparent image
img = Image.new('RGBA', (256, 512), (0, 0, 0, 0))
draw = ImageDraw.Draw(img)

# Define colors for different layers
colors = [
    (135, 206, 235, 255),  # Sky blue for background
    (34, 139, 34, 255),    # Forest green for middleground
    (139, 69, 19, 255)     # Saddle brown for foreground
]

# Draw 32x32 tiles
for i, color in enumerate(colors):
    x = (i % 8) * 32
    y = (i // 8) * 32
    draw.rectangle([x, y, x + 31, y + 31], fill=color, outline=(0, 0, 0, 255))

# Ensure the directory exists
os.makedirs(os.path.dirname("../assets/textures/background_tiles.png"), exist_ok=True)

# Save the image
img.save("../assets/textures/background_tiles.png")
