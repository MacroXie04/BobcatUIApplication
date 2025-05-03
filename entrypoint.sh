#!/bin/bash

# 确保Xpra日志目录可写
mkdir -p /tmp/xpra && chown theia:theia /tmp/xpra

# 启动Xpra服务
xpra start :100 \
    --bind-tcp=0.0.0.0:8964 \
    --html=on \
    --start-child=/home/project/bin/app \
    --exit-with-children \
    --daemon=no \
    --log-dir=/tmp/xpra \
    --verbose=debug &

# 启动Theia IDE
exec node /home/theia/applications/browser/lib/backend/main.js /home/project \
    --hostname=0.0.0.0 \
    --port=3000