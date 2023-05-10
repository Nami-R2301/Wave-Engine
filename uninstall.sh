#!/bin/bash

# Clean the working directories
sudo rm -rf /opt/Wave-Engine && echo "Deleted all files in /opt/Wave-Engine"
sudo rm -rf /usr/local/Wave-Engine && echo "Deleted all files in /usr/local/Wave-Engine"

# Remove shortcuts
sudo rm -rf /usr/bin/wave-engine && echo "Removed symbolic link to /usr/bin/wave-engine"
sudo rm -rf /usr/local/bin/wave-engine && echo "Removed symbolic link to /usr/local/bin/wave-engine"

exit 0
