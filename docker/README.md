# Prerequisite
* You need to have docker in Mac from here: https://www.docker.com/products/docker-desktop to your Download folders
* You need to download this folder to your download folder
* You need to set up xquartz by following these steps:
1. Run: "xhost +" in a terminal. If you see "Access control disabled, clients can connect from any host" then move on to the next step.
2. Ignore this step if you already have brew. You can check if you have brew by typing brew in the terminal, else download brew by running:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
3. Ignore this step if you already have xquartz. You can check if you have brew by typing brew in the terminal, else download brew by running:
```
brew install --cask xquartz
``` 
4. Open xquartz, go to XQuartz>preferences menu in the top left, choose the Security tab, and check allow connections from network clients. 
Make sure authenticate connections is unchecked
5. Completely quit out of xquartz via pressing and holding the icon in your taskbar
6. Start xquartz again, a shortcut to do so is to press CMD+Space to open search menu, typing xquartz, and then press enter.
7. Go to first time docker setup

# Fist Time Docker Setup
Note that the next few commands are only for first time setup. DO NOT DO ANY OF THOSE STEPS AGAIN YOU WILL LOSE YOUR DATA.
Run them line by line.
```
cd ~/Downloads/docker
docker pull zbirenbaum/sealgui
docker build . --tag imglab
./rundocker.sh
```

# Returning User
5: to reopen the container run:
docker start -a -i `docker ps -q -l`
or:
./openprogram.sh

# Open the GUI:
1. Run `./sealbuild` to build the program (only need to do this the first time)
2. Run `./run/imglab` to open the GUI
