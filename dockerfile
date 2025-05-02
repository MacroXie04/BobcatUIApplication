################################################################################################
# Stage 1: Build Bobcat UI Application via Makefile
################################################################################################
FROM debian:bookworm AS builder

RUN apt-get update && apt-get install -y \
      build-essential g++ clang cmake make \
      gcc-12 g++-12 gcc-11 g++-11 gcc-10 g++-10 gcc-9 g++-9 gcc-8 g++-8 gcc-7 g++-7 gcc-6 g++-6 gcc-5 g++-5 gcc-4.9 g++-4.9 \
      python3 python3-pip \
      fltk1.3-dev libgl1-mesa-dev libglu1-mesa-dev \
      libx11-dev libpng-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .


################################################################################################
# Stage 2: Theia 1.51.0 + Xpra
################################################################################################
FROM ghcr.io/eclipse-theia/theia-blueprint/theia-ide:1.51.0

USER root

# install Xpra & friends via apt
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
      xpra \
      xauth \
      x11-xserver-utils \
      xvfb \
 && rm -rf /var/lib/apt/lists/*

ENV DISPLAY=:100

# copy in your workspace (including bin/app from builder)
COPY --from=builder /app /home/project

# expose Theia and Xpra ports
EXPOSE 3000
EXPOSE 8964

# copy + normalize + chmod entrypoint
COPY docker-entrypoint.sh /usr/local/bin/docker-entrypoint.sh
RUN sed -i 's/\r$//' /usr/local/bin/docker-entrypoint.sh \
 && chmod +x /usr/local/bin/docker-entrypoint.sh

ENTRYPOINT ["/usr/local/bin/docker-entrypoint.sh"]
CMD []