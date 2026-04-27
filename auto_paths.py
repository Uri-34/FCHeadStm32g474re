import os
Import("env")

PROJECT_DIR = env.get("PROJECT_DIR")

# Директории, которые нужно полностью игнорировать при поиске
# Добавлены TEMP и TEST. Проверка будет нечувствительна к регистру.
EXCLUDE = {".git", ".pio", "build", "mbed-os", ".vscode", ".settings", "libdeps", "TEMP", "TEST"}
EXCLUDE_UPPER = {e.upper() for e in EXCLUDE}

custom_includes = []
custom_src_filters = []

for root, dirs, files in os.walk(PROJECT_DIR):
    # Фильтруем директории "на лету", чтобы os.walk не рекурсивно заходил внутрь
    dirs[:] = [
        d for d in dirs 
        if d.upper() not in EXCLUDE_UPPER and not d.startswith('.')
    ]
    
    rel = os.path.relpath(root, PROJECT_DIR).replace("\\", "/")

    # 1. Добавляем папку в include path (кроме корня проекта, он уже в системе)
    if rel != ".":
        custom_includes.append(root)

    # 2. Если в папке есть исходники – добавляем в фильтр сборки
    if any(f.endswith(('.c', '.cpp', '.cc', '.S', '.s', '.sx')) for f in files):
        if rel != ".":
            custom_src_filters.append(f"+<{rel}/*>")

# Внедряем найденные пути и фильтры в систему сборки PlatformIO (SCons)
env.Append(CPPPATH=custom_includes)
env.Append(SRC_FILTER=custom_src_filters)
