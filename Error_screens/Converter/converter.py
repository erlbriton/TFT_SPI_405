#!/usr/bin/env python3
import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image
import os
import re

def to_camel_case(text):
    words = re.findall(r'[a-zA-Z0-9]+', text)
    if not words:
        return "imageArray"
    return words[0].lower() + ''.join(word.capitalize() for word in words[1:])

def convert_to_rgb565():
    file_path = filedialog.askopenfilename(filetypes=[("Image files", "*.bmp *.png *.jpg *.jpeg")])
    if not file_path:
        return

    try:
        img = Image.open(file_path).convert('RGB')
        width, height = img.size
        pixels = list(img.getdata())

        raw_name = os.path.splitext(os.path.basename(file_path))[0]
        camel_name = to_camel_case(raw_name)
        output_name = f"{camel_name}.c"

        with open(output_name, 'w') as f:
            f.write(f"// Image size: {width}x{height}\n\n")
            # Убрали #include <stdint.h> по просьбе пользователя
            f.write(f"const uint16_t {camel_name}[{len(pixels)}] = {{\n    ")

            for i, (r, g, b) in enumerate(pixels):
                rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
                f.write(f"0x{rgb565:04X}, ")

                if (i + 1) % 12 == 0 and i < len(pixels) - 1:
                    f.write("\n    ")

            f.write("\n};\n")

        messagebox.showinfo("Готово", f"Файл: {output_name}\nМассив: {camel_name}")
    except Exception as e:
        messagebox.showerror("Ошибка", f"Ошибка:\n{e}")

root = tk.Tk()
root.title("RGB565 Converter")
root.geometry("350x150")

label = tk.Label(root, text="Конвертер в RGB565 (CamelCase)", pady=20)
label.pack()

btn = tk.Button(root, text="Выбрать файл", command=convert_to_rgb565, padx=20, pady=10)
btn.pack()

root.mainloop()
