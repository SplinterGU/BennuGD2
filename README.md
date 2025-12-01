[![Build and Release BennuGD2 Packages](https://github.com/SplinterGU/BennuGD2/actions/workflows/build-and-release.yml/badge.svg)](https://github.com/SplinterGU/BennuGD2/actions/workflows/build-and-release.yml)
[![Wiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/SplinterGU/BennuGD2)
![GitHub all releases](https://img.shields.io/github/downloads/SplinterGU/BennuGD2/total)

# BennuGD2

Welcome to BennuGD2! BennuGD2 is a game development system focused on ease of use and portability.

## Setup Environment

### For Unix

To set up your environment for BennuGD2, add the following environment variables to your shell profile (e.g., `.bashrc`, `.zshrc`):

```bash
# BENNUGD2 DEV

# Set the path to your BennuGD2 installation directory
export BGD2DEV=<path_to_your_BennuGD2_installation_directory>

# Setup for Linux systems
export PATH=$BGD2DEV/build/linux-gnu/bin:$PATH
export LD_LIBRARY_PATH=$BGD2DEV/build/linux-gnu/bin:$LD_LIBRARY_PATH

# For other platforms:
# Replace `linux-gnu` in the paths according to the desired platform.

# aarch64-none-elf (Nintendo Switch, unofficial scene version)
# export PATH=$BGD2DEV/build/aarch64-none-elf/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/aarch64-none-elf/bin:$LD_LIBRARY_PATH

# i386-linux-gnu
# export PATH=$BGD2DEV/build/i386-linux-gnu/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/i386-linux-gnu/bin:$LD_LIBRARY_PATH

# i686-w64-mingw32
# export PATH=$BGD2DEV/build/i686-w64-mingw32/bin:$PATH

# x86_64-apple-darwin14 (macOS)
# export PATH=$BGD2DEV/build/x86_64-apple-darwin14/bin:$PATH
# export DYLD_LIBRARY_PATH=$BGD2DEV/build/x86_64-apple-darwin14/bin:$DYLD_LIBRARY_PATH

# x86_64-w64-mingw32
# export PATH=$BGD2DEV/build/x86_64-w64-mingw32/bin:$PATH
```

Replace `<path_to_your_BennuGD2_installation_directory>` with the actual path where BennuGD2 is installed on your system.

## How to Build BennuGD2

### Unix - Ubuntu/Debian

1. **Install Essential Packages**

   For Ubuntu, you may need to install the following packages:

   ```bash
   sudo apt install binutils git cmake build-essential zlib1g-dev libsdl2-dev libglu1-mesa-dev libsdl2-image-dev libsdl2-mixer-dev libtheora-dev libogg-dev libvorbis-dev
   ```

   For other distributions, install the equivalent packages:

   - **Fedora**:
     ```bash
     sudo dnf install binutils git cmake gcc-c++ zlib-devel SDL2-devel mesa-libGLU-devel SDL2_image-devel SDL2_mixer-devel libtheora-devel libogg-devel libvorbis-devel
     ```
   - **Arch Linux**:
     ```bash
     sudo pacman -S git cmake gcc zlib sdl2 glu sdl2_image sdl2_mixer libtheora libogg libvorbis
     ```
   - **openSUSE**:
     ```bash
     sudo zypper install git cmake gcc-c++ zlib-devel SDL2-devel libglvnd-devel libSDL2_image-devel libSDL2_mixer-devel libtheora-devel libogg-devel libvorbis-devel
     ```

2. **Clone and Build BennuGD2**

   Run the following commands to clone the repository and build the dependencies:

   ```bash
   git clone https://github.com/SplinterGU/BennuGD2.git

   cd BennuGD2/

   git submodule update --init --recursive

   cd vendor

   ./build-sdl-gpu.sh linux clean

   cd ..
   ./build.sh linux clean
   ```

### Windows 64

1. **Install MSYS2**

   - Download the latest version of MSYS2 from the [official website](https://www.msys2.org/).
   - Follow the installation instructions provided on the website for your specific operating system.
   - Open the MSYS2 terminal to begin the installation of BennuGD2.

2. **Build BennuGD2**

   Copy and paste the following script into your MSYS2 terminal:

   ```bash
   pacman -S --needed --noconfirm git patch cmake mingw-w64-x86_64-toolchain mingw-w64-x86_64-pkg-config mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-emacs mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-libpng mingw-w64-x86_64-zlib mingw-w64-x86_64-libogg mingw-w64-x86_64-libvorbis mingw-w64-x86_64-libtheora mingw-w64-x86_64-libmodplug mingw-w64-x86_64-libmikmod mingw-w64-x86_64-libtre-git mingw-w64-x86_64-flac mingw-w64-x86_64-openal mingw-w64-x86_64-libxml2 mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libwebp

   git clone https://github.com/SplinterGU/BennuGD2.git

   cd BennuGD2/

   git submodule update --init --recursive

   cd vendor

   ./build-sdl-gpu.sh windows

   cd ..
   ./build.sh windows
   cp dependencies/x86_64-w64-mingw32/* build/x86_64-w64-mingw32/bin/
   ```

   **Note:** Adjust the `cp` command above to match the platform on which the build was performed.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
