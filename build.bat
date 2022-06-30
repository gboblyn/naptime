@echo off
cd build
cl /I ..\deps ../src/win32_entry.c ../src/win32_vulkan_loader.c user32.lib xinput.lib gdi32.lib /Fegfg
cd .. 
