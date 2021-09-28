#!/bin/bash

patch -b sdl-gpu/src/renderer_GL_common.inl < sdl-gpu-odd-texture-fix.patch
