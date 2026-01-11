#!/bin/bash
./makemk.sh
mk nuke
mk install 2>&1 | tee install.log
