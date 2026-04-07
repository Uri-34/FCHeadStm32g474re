# scripts/force_cpp.py
Import("env")

# Найти и заменить компиляцию lv_hal_tick.c на C++
for i, src in enumerate(env["SOURCES"]):
    if "lv_hal_tick.c" in src:
        env["SOURCES"][i] = src.replace(".c", ".cpp")
        env.Replace(
            CC=env["CXX"],
            CCFLAGS=env["CXXFLAGS"]
        )
        break
