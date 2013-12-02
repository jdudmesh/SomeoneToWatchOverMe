#!/usr/bin/env python
import os
import shutil

root = '/var/folders/1w/tmlss7nj49d_c1wm5bsw0f400000gn/T/build8479179664445173395.tmp'
outdir = "/Users/john/Documents/workspace-kepler/SomeoneToWatchOverMe/lib"

def copyFile(src):
	p = os.path.split(src)
	dst = os.path.join(outdir, p[1])
	print src
	print dst
	print
	shutil.copyfile(src, dst)
	
def parseFile(path):
	f = open(path, "r")
	for line in f:
		filename = line.strip()[:-2]
		print ">>>" + filename
		if filename.startswith("/") and (filename.endswith(".cpp") or filename.endswith(".h")) :
			copyFile(filename)
		
	f.close()
	
def parseDir(path):
	
	for root, dirs, files in os.walk(path):
		#print root, dirs, files
		for file in files:
			if file.endswith(".c.d") or file.endswith(".cpp.d"):
				parseFile(root + "/" + file)
				
				
parseDir(root)
copyFile(os.path.join(root, "core.a"))