#!/bin/sh
set -e

# 1) Start Xpra as a daemon, launching your built app and staying alive until it exits:
xpra start :100 \
    --bind-tcp=0.0.0.0:8964 \
    --daemon=yes \
    --exit-with-children=yes \
    --html=on \
    --start-child=/home/project/bin/app

# 2) Exec Theia’s backend from the Blueprint layout:
#    the correct path is /home/theia/applications/browser/lib/backend/main.js  [oai_citation:0‡GitHub](https://github.com/eclipse-theia/theia-blueprint/blob/master/browser.Dockerfile?utm_source=chatgpt.com)
exec node /home/theia/applications/browser/lib/backend/main.js /home/project \
     --hostname=0.0.0.0 \
     --port=3000