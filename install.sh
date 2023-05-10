#!/bin/bash

# Build using CMake
mkdir build
cmake -S . -B build
cd build
sudo make install

# Setup working directory for Engine to load assets properly
sudo mkdir -p /opt/Wave-Engine/Wave && sudo mkdir -p /opt/Wave-Engine/Wave-Editor
sudo cp -R ../Wave/res/ /opt/Wave-Engine/Wave
sudo cp -R ../Wave-Editor/res/ /opt/Wave-Engine/Wave-Editor

# Setup permissions to be able to read ressources and write to imgui.ini and wave-engine.ini
sudo chmod -R 777 /opt/Wave-Engine/*

# Setup shortcut for Wave-Engine's executable
sudo echo "#!/bin/bash" >> /usr/bin/wave-engine
sudo echo "cd /opt/Wave-Engine/bin" >> /usr/bin/wave-engine
sudo echo "./Editor" >> /usr/bin/wave-engine
sudo echo 'exit $?' >> /usr/bin/wave-engine
sudo chmod +rwx /usr/bin/wave-engine

exit 0
