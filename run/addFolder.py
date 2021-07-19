import os 
import sys
from pathlib import Path

def main():
    args = sys.argv
    folders = args[1]
    txtFile = args[2]
    
    file = open(txtFile, 'a')
    folderLists = "picfolder="
    
    for i, folder in enumerate(os.listdir(folders)):
        currentPath = os.path.join(folders, folder)
        if not os.path.isdir(currentPath):
            continue
        if (i == len(os.listdir(folders))): 
            folderLists += str(currentPath)
        else:
            folderLists += str(currentPath) + " "
    file.write(folderLists)
    file.close()
    
if __name__ == '__main__':
    main()