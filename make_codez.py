#!/usr/bin/env python

# makes .code files wherever .err output is not empty
#python 2.7 vs python 3
import sys
if sys.version_info < (3, 0):
    def console_input(prompt):
        return raw_input(prompt)
else:
    def console_input(prompt):
        return input(prompt)

import glob
import re
import os.path
import sys

def file_or_none(filename, dir):
    if os.path.isfile(dir+filename):
        return filename
    else:
        return None

# This handles top level exceptions so that the window doesn't close
def show_exception_and_exit(exc_type, exc_value, tb):
    import traceback
    traceback.print_exception(exc_type, exc_value, tb)
    console_input("Press key to exit.")
    sys.exit(-1)
sys.excepthook = show_exception_and_exit

test_name = ""
test_dir = ""
if len(sys.argv) > 1:
    if os.path.isdir(sys.argv[1]):
        test_dir = sys.argv[1]
    else:
        print("ERROR: path ", path, " invalid!")
if len(sys.argv) > 2:
    test_name =  sys.argv[2]
# Get name of the HW and list of innuts
if len(test_dir) == 0:
    test_dir = console_input("Enter directory:")
if not test_dir.endswith("/"):
    test_dir += "/"
    
pattern = test_dir+"*.err"
print("glob.glob(\"", pattern, "\")")
errs = glob.glob(test_dir+"*.err")
print(errs)
filename_regex = re.compile("[/\\\\](([^\\\\/]+)\.err)$")

# Find outputs for inputs
for filename in errs:
    print("File: ", filename)
    matches = filename_regex.search(filename)
    case_name = matches.group(2);
    
    retcode = file_or_none(case_name+".code", test_dir)
    if retcode is None:
        if os.path.getsize(filename)>0:
            print("    Creating .code file.")
            text_file = open(test_dir+case_name+".code", "w")
            text_file.write("100")
            text_file.close()
