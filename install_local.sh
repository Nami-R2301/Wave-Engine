#!/bin/bash

# Build using CMake
mkdir -p build
cmake -S . -B build
cd build
cmake --build . --target Editor -- -j

# Setup working directory for Engine to load assets properly
sudo mkdir -p /usr/local/Wave-Engine/bin
sudo cp -R Editor /usr/local/Wave-Engine/bin
sudo mkdir -p /usr/local/Wave-Engine/Wave && sudo mkdir -p /usr/local/Wave-Engine/Wave-Editor
sudo cp -R ../Wave/res/ /usr/local/Wave-Engine/Wave
sudo cp -R ../Wave-Editor/res/ /usr/local/Wave-Engine/Wave-Editor

# Setup permissions to be able to read ressources and write to imgui.ini and wave-engine.ini
sudo chmod -R +rx /usr/local/Wave-Engine/*
sudo chmod -R +rwx /usr/local/Wave-Engine/bin

# Setup shortcut for Wave-Engine's executable
sudo echo "#!/bin/bash" >> /usr/local/bin/wave-engine
sudo echo "cd /usr/local/Wave-Engine/bin" >> /usr/local/bin/wave-engine
sudo echo "./Editor" >> /usr/local/bin/wave-engine
sudo echo 'exit $?' >> /usr/local/bin/wave-engine
sudo chmod +rwx /usr/local/bin/wave-engine

exit 0
