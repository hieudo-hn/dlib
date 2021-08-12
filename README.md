# SealNet FaceChipping 
This project is based on Davis King's Imglab & source code of dlib - a C++ machine learning toolkit.

## Prerequisite to running the program:
If you are a returning user, you can skip this section and go to the returning user section.

1. Clone/Download this repo into Desktop by first opening your terminal:
```
cd ~/Desktop
git clone https://github.com/hieudo-hn/dlibSealGUI.git
```
### First Time XQuartz Setup
* You need to have docker in Mac from here: https://www.docker.com/products/docker-desktop. If you are on Mac devices, you can click on the Apple Icon > "About This Mac" to know which chip you have (Intel or Apple chip).

* You need to set up xquartz by following these steps:
1. Run: `xhost +`. If you see "Access control disabled, clients can connect from any host" then move on to the next step.
2. Ignore this step if you already have brew. You can check if you have brew by typing brew in the terminal, else download brew by running:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install --cask xquartz
``` 
3. Download XServer
  + If typing `xquartz` does not output anything, xquartz is installed, you can type CTRL+c and move on to the next step
  + If it returns `command not found` run the following to install XServer:
    + (MacOS) `brew install --cask xquartz`
    + (General) WSL: Setup VcXsrv, or wslg from microsoft preview builds

4. Open xquartz (type CMD+spacebar and search for XQuartz), go to XQuartz>preferences menu in the top left, choose the Security tab
  + check allow connections from network clients. 
  + Make sure authenticate connections is unchecked
5. Completely quit out of xquartz via pressing and holding the icon in your taskbar
6. Start xquartz again, a shortcut to do so is to press CMD+Space to open search menu, typing xquartz, and then press enter.
7. Go to first time docker setup

### First Time Docker Setup
Note that the next few commands are only for first time setup. Run them line by line.
```
cd ~/Downloads/dlibSealGUI/docker
./initializedocker.sh
```

## Returning User 
1. Open the terminal and go to this repo by running:
```
cd ~/Desktop/dlibSealGUI
```
2. Run `git pull` to get any updates
3. To reopen the container (e.g. run the program) run:
```
docker start -a -i `docker ps -q -l`
```
or:
``` 
cd ~/Downloads/dlibSealGUI/docker
./runprogram.sh
```

## Important files:
1. trainedModel/final.dat: Trained model using all of our training data to use for face detection. For more information on how to train the model, visit https://github.com/hieudo-hn/SealChipModel

2. run/config.txt: Configuration file. The config.txt file should be written as follow:
```
xml=XMLFILE //to store face chip data
model=FACECHIPMODEL //.dat file of a trained face chipping model (default=seal.dat)
picfolder=PHOTOFOLDER1 PHOTOFOLDER2 PHOTOFOLDER3 // list of relative paths to photo directories that you want to chip, separated by 1 space blank.
```
An example of a config.txt file:
```
xml=data.xml
model=seal.dat
picfolder=Whaleboat_1_1 Mitchell_Field_1_31
```
Note: picfolder contains the relative path to the photo directory. If you are not familiar with directory pathing, move all of your photo directories into the "run" folders and put the name of those directories in picfolder like in the above example

3. run/final.xml: XML file contains all of the metadata on our training dataset (including truth boxes and parts' location)

## Important folders:
1. imglab/src: source code to run imglab. Most of the edits are in /imglab/src/metadata_editor.cpp (includes automatic chipping and auto alignment)

2. include/dlib: dlib source code (check http://dlib.net/ for more information)

3. sealdnn: contains dnn_seal_chip.cpp to execute face-chipping and store the result in the xmlFile (you can call this executable in the command line or in the GUI)

4. run: folder containing all of the executable to run the program

5. trainedModel: containing all of the trained face detection model to start automatic chipping

6. data: where your dataset should be stored.

## How to use the program:
You can use imglab to mark the bounding boxes that indicate the seal faces to be trained. 
Imglab is a simple graphical tool for annotating images with object bounding
boxes and optionally their part locations.  Generally, you use it when you want
to train an object detector (e.g. a face detector) since it allows you to
easily create the needed training dataset.

One you follow the previous instructions to run the program, you will see a GUI window that has all
the photos in the folders specified in config.txt. You can use the up and down
arrow keys to cycle though the images and the mouse to label objects.  In
particular, holding the shift key, left clicking, and dragging the mouse will
allow you to draw boxes around the objects you wish to detect. 

To use the chipping software, click Chip >> Start. It might take a while if your laptop does not
have a GPU. It is best to use an overnight run.

To better understand the program, you can click About >> Help and read the detailed instructions.

* Note: to increase the accuracy of your model, some faces in the photos might be cut/blurry/of bad quality. If you are unsure 
whether your model should detect such faces or not, indicate "ignore" option on such bounding boxes by clicking on the box and 
press "i". Example:
![Imglab Example](https://ibb.co/sj3pTpQ)

## Additional information:
[GUI Extensions and Functionality](docs/GUI.md)

[Searching Source Code For Specifics](docs/SEARCH.md)

You can look into Dlib's source code in http://dlib.net/. 




