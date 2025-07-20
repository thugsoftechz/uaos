$base = "C:\Users\meena\OneDrive\Documents\uaos"
$dirs = @(
    "src/boot", "src/kernel", "src/drivers", "src/gui", "src/apps", "src/lib",
    "include", "build"
)

foreach ($dir in $dirs) {
    New-Item -ItemType Directory -Path (Join-Path $base $dir) -Force
}

# Optionally, create placeholder files
$files = @(
    "src/boot/bootloader.asm", "src/boot/boot.asm",
    "src/kernel/kernel.c", "src/kernel/kernel.h", "src/kernel/memory.c", "src/kernel/process.c", "src/kernel/interrupts.c",
    "src/drivers/keyboard.c", "src/drivers/vga.c", "src/drivers/disk.c",
    "src/gui/gui.c", "src/gui/window.c", "src/gui/events.c",
    "src/apps/file_manager.c", "src/apps/browser.c", "src/apps/media_player.c", "src/apps/package_manager.c",
    "src/lib/string.c", "src/lib/stdlib.c", "src/lib/syscalls.c",
    "include/kernel.h", "include/memory.h", "include/gui.h", "include/apps.h", "include/drivers.h",
    "build/Makefile", "build/linker.ld",
    "README.md"
)

foreach ($file in $files) {
    New-Item -ItemType File -Path (Join-Path $base $file) -Force
}