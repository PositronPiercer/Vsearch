import os
import time

if __name__ == "__main__":
    os.system ("mkdir data/inv_index")
    os.system ("cp data/korg data/keywords")
    #print ("Enter source directory path")
    #sourceDir = input ()
    sourceDir="data/test_files"
    fileNames = os.listdir(sourceDir)

    for name in fileNames:
        os.system("./binv " + os.path.join (sourceDir, name))
        time.sleep (1)

    os.system ("rm data/signatures")
    print ("Inverted Index built successfully")
