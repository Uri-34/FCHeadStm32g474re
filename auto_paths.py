import os
Import("env")

PROJECT_DIR = env.get("PROJECT_DIR")
# Директории, которые mbed/platformio уже обрабатывают или которые нужно игнорировать
EXCLUDE = {".git", ".pio", "build", "mbed-os", ".vscode", ".settings", "libdeps"}

custom_includes = []
custom_src_filters = []

for root, dirs, files in os.walk(PROJECT_DIR):
    # Исключаем системные папки
    dirs[:] = [d for d in dirs if d not in EXCLUDE and not d.startswith('.')]
    rel = os.path.relpath(root, PROJECT_DIR).replace("\\", "/")

    # 1. Добавляем все найденные папки в include path
    custom_includes.append(root)

    # 2. Если в папке есть исходники – добавляем в фильтр сборки
    if any(f.endswith(('.c', '.cpp', '.cc', '.S', '.s')) for f in files):
        custom_src_filters.append(f"+<{rel}/*>")

# Внедряем в среду сборки PlatformIO
env.Append(CPPPATH=custom_includes)
env.Append(SRC_FILTER=custom_src_filters)
