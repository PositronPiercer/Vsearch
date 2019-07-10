import os
import time

if __name__ == "__main__":
    os.system ("mkdir inv_index")
    os.system ("cp korg keywords")
    print ("Enter source directory path")
    sourceDir = input ()
    fileNames = os.listdir(sourceDir)

    for name in fileNames:
        os.system("./binv " + os.path.join (sourceDir, name))
        time.sleep (1)

    os.system ("rm signatures")
    print ("Inverted Index built successfully")
