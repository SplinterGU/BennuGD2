# BennuGD2

Welcome to BennuGD2! BennuGD2 is a game development system focused on ease of use and portability.

## How to Build BennuGD2

### Unix - Ubuntu/Debian

**Step 1: Build SDL_gpu**

1. Go to the `vendor` directory and run the `sdl-gpu-patch-*.sh` scripts.
2. Then run `./build-sdl-gpu.sh` with appropriate options, for example:
   ```
   ./build-sdl-gpu.sh linux clean
   ```

**Step 2: Build BennuGD2**

1. Navigate to the project root and run `./build.sh` with suitable options:
   ```
   ./build.sh linux clean
   ```

For Ubuntu, you may need to install the following packages:

```bash
sudo apt install binutils git cmake build-essential zlib1g-dev libsdl2-dev libglu1-mesa-dev libsdl2-image-dev libvlc-dev libsdl2-mixer-dev
```

### Other Distros

For other distributions, you might need to install the equivalent packages:

- **Fedora**:
  ```bash
  sudo dnf install binutils git cmake gcc-c++ zlib-devel SDL2-devel mesa-libGLU-devel SDL2_image-devel libvlc-devel SDL2_mixer-devel
  ```

- **Arch Linux**:
  ```bash
  sudo pacman -S git cmake gcc zlib sdl2 glu sdl2_image vlc sdl2_mixer
  ```

- **openSUSE**:
  ```bash
  sudo zypper install git cmake gcc-c++ zlib-devel SDL2-devel libglvnd-devel libSDL2_image-devel libvlc-devel libSDL2_mixer-devel
  ```

You can adapt these commands based on the package managers and package names of your specific Linux distribution.

### Windows 64

**Step 1: Install MSYS2**

1. Download the latest version of MSYS2 from the [official website](https://www.msys2.org/).
2. Follow the installation instructions provided on the website for your specific operating system.
3. Open the MSYS2 terminal to begin the installation of BennuGD2.

**Step 2: Execute the Script**

Copy and paste the provided script into your MSYS2 terminal. This script will install necessary dependencies and build BennuGD2.

```bash
#!/bin/sh
# base-devel msys2-devel
pacman -S --needed --noconfirm git patch cmake mingw-w64-x86_64-toolchain mingw-w64-x86_64-pkg-config mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-emacs mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer mingw-w64-x86_64-libpng mingw-w64-x86_64-zlib mingw-w64-x86_64-libogg mingw-w64-x86_64-libvorbis mingw-w64-x86_64-libtheora mingw-w64-x86_64-libmodplug mingw-w64-x86_64-libmikmod mingw-w64-x86_64-libtre-git mingw-w64-x86_64-flac mingw-w64-x86_64-openal mingw-w64-x86_64-libxml2 mingw-w64-x86_64-libjpeg-turbo mingw-w64-x86_64-libwebp

git clone https://github.com/SplinterGU/BennuGD2.git

cd BennuGD2/

git submodule update --init --recursive

cd vendor

./sdl-gpu-patch-001.sh
./sdl-gpu-patch-002.sh

./build-sdl-gpu.sh windows

cd ..
./build.sh windows
cp dependencies/x86_64-w64-mingw32/* build/x86_64-w64-mingw32/bin/
```

This should successfully build BennuGD2 for Windows using MSYS2.

## Setup Environment

### For Unix

Add the following environment variables to your shell profile (e.g., `.bashrc`, `.zshrc`) for your BennuGD2 installation:

```bash
# BENNUGD2 DEV

# Set the path to your BennuGD2 installation directory
export BGD2DEV=<path_to_your_BennuGD2_installation_directory>

# Setup for Linux systems
export PATH=$BGD2DEV/build/linux-gnu/bin:$PATH
export LD_LIBRARY_PATH=$BGD2DEV/build/linux-gnu/bin:$LD_LIBRARY_PATH

# For other platforms:
# Replace `linux-gnu` in the paths according to the desired platform.

# aarch64-none-elf
# export PATH=$BGD2DEV/build/aarch64-none-elf/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/aarch64-none-elf/bin:$LD_LIBRARY_PATH

# i386-linux-gnu
# export PATH=$BGD2DEV/build/i386-linux-gnu/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/i386-linux-gnu/bin:$LD_LIBRARY_PATH

# i686-w64-mingw32
# export PATH=$BGD2DEV/build/i686-w64-mingw32/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/i686-w64-mingw32/bin:$LD_LIBRARY_PATH

# x86_64-apple-darwin14
# export PATH=$BGD2DEV/build/x86_64-apple-darwin14/bin:$PATH
# export DYLD_LIBRARY_PATH=$BGD2DEV/build/x86_64-apple-darwin14/bin:$DYLD_LIBRARY_PATH

# x86_64-w64-mingw32
# export PATH=$BGD2DEV/build/x86_64-w64-mingw32/bin:$PATH
# export LD_LIBRARY_PATH=$BGD2DEV/build/x86_64-w64-mingw32/bin:$LD_LIBRARY_PATH
```

Please replace `<path_to_your_BennuGD2_installation_directory>` with the actual path where BennuGD2 is installed on your system.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
