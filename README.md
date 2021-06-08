## SealNet FaceChipping 
This project is based on Davis King's Imglab & source code of dlib - a C++ machine learning toolkit.

# Prerequisite to running the program:
1. Clone/Download this repo:
```
git clone https://github.com/hieudo-hn/dlibSealGUI.git
```
or download the zip file and unzip it in your laptop if you do not have git.

2. Download C++ Compiler:
You can download C++ Compiler using this link: https://gcc.gnu.org/. Make sure to download a compiler that is compatible with your operating system, for Windows you might need to add a PATH to make it work. You can skip this step if you already have a compiler.

3. Download CMake:
You can download CMake using this link: https://cmake.org/download/. Make sure to download CMake that is compatible with your operating system. You can skip this step if CMake is already installed.

# Important folders:
1. imglab/src: source code to run imglab

2. include/dlib: dlib source code

3. include/dnn: contains sealFindr.cpp and dnn_seal_chip.cpp to execute face-chipping and store the result in the xmlFile

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

2. run `buildseal` to compile all of the programs. `buildseal` can be called anywhere in your local repo.

3. `cd run`

4. Edit config.txt file using an editor or using `nano config.txt` in the terminal. At the end you can do Ctrl-X then Y and hit Enter to save. Please follow the guidelines in writing config.txt file for our programs to run smoothly.

5. run the program with `./imglab`

# Additional information:
[GUI Extensions and Functionality](docs/GUI.md)
[Searching Source Code For Specifics](docs/SEARCH.md)
You can look into Dlib's source code in http://dlib.net/. 




