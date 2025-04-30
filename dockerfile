################################################################################################
# Project: Bobcat UI Application Project
# Author: Hongzhe Xie
# Date: April 2025
# Description:
#   This Docker simulation the environment of Steam Plug.
################################################################################################

# ----------------------------------------------------------------------------------------------
# Step 1: Use Debian 12 (Bookworm) as the base image
# ----------------------------------------------------------------------------------------------
FROM debian:bookworm

# ----------------------------------------------------------------------------------------------
# Step 2: Install all necessary build tools, libraries, and Xpra dependencies
# ----------------------------------------------------------------------------------------------
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    clang \
    cmake \
    fltk1.3-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libpng-dev \
    xpra \
    wget \
    xauth \
    x11-xserver-utils \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

# ----------------------------------------------------------------------------------------------
# Step 3: Configure environment variables
# ----------------------------------------------------------------------------------------------
ENV DISPLAY=:100

# ----------------------------------------------------------------------------------------------
# Step 4: Set up the application directory
# ----------------------------------------------------------------------------------------------
WORKDIR /app

# Copy all project files into the container
COPY . .

# ----------------------------------------------------------------------------------------------
# Step 5: Prepare source files
# ----------------------------------------------------------------------------------------------
# Search and replace macOS-specific OpenGL headers with Linux-compatible ones
RUN find . -type f \( -name '*.cpp' -o -name '*.h' \) \
      -exec sed -i -e 's|<OpenGL/gl.h>|<GL/gl.h>|g' \
                   -e 's|<OpenGL/glu.h>|<GL/glu.h>|g' {} +

# ----------------------------------------------------------------------------------------------
# Step 6: Build the application
# ----------------------------------------------------------------------------------------------
RUN mkdir -p bin objects \
 && for f in src/*.cpp; do \
      echo "Compiling $f"; \
      g++ -Wall $(fltk-config --cxxflags) -std=c++17 -c "$f" \
         -o objects/$(basename "$f" .cpp).o; \
    done \
 && echo "Linking into bin/app" \
 && g++ objects/*.o -o bin/app \
        $(fltk-config --ldflags) -lfltk_gl -lfltk_images -lGL -lGLU


# ----------------------------------------------------------------------------------------------
# Step 7: Launch Xpra server and start the application
# ----------------------------------------------------------------------------------------------
# Bind the Xpra server strictly to localhost on port 8964
CMD xpra start :100 \
    --bind-tcp=0.0.0.0:8964 \
    --start-child=./bin/app \
    --exit-with-children \
    --daemon=no