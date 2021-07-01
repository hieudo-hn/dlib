## SealNet FaceChipping 
This project is based on Davis King's Imglab & source code of dlib - a C++ machine learning toolkit.

# Prerequisite to running the program:
1. Clone/Download this repo:
```
git clone https://github.com/hieudo-hn/dlibSealGUI.git
```
or download the zip file and unzip it in your laptop if you do not have git.

2. (MacOS) Check if you have homebrew
If typing `brew` in your terminal returns `command not found` run the following to install homebrew:
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

3. Download C++ Compiler:

  + You can download C++ Compiler using this link: https://gcc.gnu.org/. Make sure to download a compiler that is compatible with your operating system, for Windows you might need to add a PATH to make it work. You can skip this step if you already have a compiler.

4. Download CMake:

  + (MacOS) Just run `brew install cmake`
  + (Others/General) You can download CMake using this link: https://cmake.org/download/. Make sure to download CMake that is compatible with your operating system. You can skip this step if CMake is already installed.

5. Download Ninja:

  + (MacOS) Just run `brew install ninja`
  + (Other/General) Follow the instructions on this page: https://ninja-build.org/

6. Download XServer
  + (MacOS) `brew install --cask xquartz`
  + (General) WSL: Setup VcXsrv, or wslg from microsoft preview builds

# Important folders:
1. imglab/src: source code to run imglab

2. include/dlib: dlib source code

3. sealdnn: contains sealFindr.cpp and dnn_seal_chip.cpp to execute face-chipping and store the result in the xmlFile

4. run: folder to run the program

# Important files:
1. run/seal.dat: Trained model to detect seal face. For more information on how to train the model, visit https://github.com/hieudo-hn/SealChipModel

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

# Running the program:
1. `cd` to your local repository

2. run `sealbuild` or `sealbuildAWS` (if you run this program on the AWS machine) in the main project directory (the one with imglab, run, etc) to build all of the programs and place them in the run folder. 

  + (Optional) To run the build command from anywhere navigate to the main project directory and execute: 

  + `echo "export PATH=$PATH:$(pwd)" >> ~/.bashrc`

3. `cd run`

4. Edit config.txt file using an editor or using `nano config.txt` in the terminal. At the end you can do Ctrl-X then Y and hit Enter to save. Please follow the guidelines in writing config.txt file for our programs to run smoothly.

5. run the program with `./imglab`

# How to use the program:
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

# Additional information:
[GUI Extensions and Functionality](docs/GUI.md)

[Searching Source Code For Specifics](docs/SEARCH.md)

You can look into Dlib's source code in http://dlib.net/. 




