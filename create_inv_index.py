import os
import time

if __name__ == "__main__":
	os.system ("mkdir data/inv_index")
	sourceDir="data/test_files"
	fileNames = os.listdir(sourceDir)

	for name in fileNames:
		print(name)
		os.system("./binv " + os.path.join (sourceDir, name))
		time.sleep (1)

	print ("Inverted Index built successfully")
