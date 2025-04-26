# Use Ubuntu 22.04 as base
FROM ubuntu:22.04

# Install system packages
RUN apt update && apt install -y \
    clang \
    make \
    cmake \
    git \
    libfltk1.3-dev \
    libglu1-mesa-dev \
    libx11-dev \
    libxext-dev \
    libxft-dev \
    libxinerama-dev \
    xvfb \
    x11-apps \
    && apt clean

# Set environment variables for FLTK and graphics
ENV DISPLAY=:0

# Create working directory
WORKDIR /app

# Copy source code into container
COPY . .

# Build the application
RUN make clean && make all

# Default command: run the app
CMD ["make", "run"]