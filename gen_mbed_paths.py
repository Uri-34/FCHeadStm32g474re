#!/usr/bin/env python3
import os
import sys

# Директории, которые нужно игнорировать
EXCLUDE = {".git", ".pio", "build", "CMakeFiles", "mbed-os", ".vscode", ".settings"}
BASE_DIR = os.path.dirname(os.path.abspath(__file__))

def scan_and_generate():
    inc_flags = []
    src_filters = ["    +<src/>"]  # Оставляем стандартный поиск в src/
    
    for root, dirs, files in os.walk(BASE_DIR):
        # Фильтруем системные папки
        dirs[:] = [d for d in dirs if d not in EXCLUDE and not d.startswith('.')]
        rel = os.path.relpath(root, BASE_DIR)
        if rel == ".": rel = ""

        # 1. Собираем пути для инклудов (-I)
        if rel and rel not in ("src", "include", "lib"):
            inc_flags.append(f"    -I${{PROJECT_DIR}}/{rel}")

        # 2. Собираем фильтры исходников (если в папке есть .c/.cpp)
        if any(f.endswith(('.c', '.cpp', '.cc', '.S', '.s')) for f in files) and rel and rel not in ("src", "include"):
            src_filters.append(f"    +<{rel}/*>")

    # Формируем INI-фрагмент
    content = "; !!! AUTO-GENERATED !!! DO NOT EDIT MANUALLY\n"
    content += "; Аналог .pri для PlatformIO\n\n"
    content += "build_flags =\n" + "\n".join(inc_flags) + "\n\n"
    content += "build_src_filter =\n" + "\n".join(src_filters) + "\n"

    with open("auto_paths.ini", "w", encoding="utf-8") as f:
        f.write(content)
    print("✅ auto_paths.ini сгенерирован успешно.")

if __name__ == "__main__":
    scan_and_generate()
