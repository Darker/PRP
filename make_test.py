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
    directory = console_input("Enter directory:")
    if os.path.isdir(directory):
        test_dir = directory
if len(test_name) == 0:
    test_name = console_input("Enter test name:")
if not test_dir.endswith("/"):
    test_dir += "/"
if len(test_dir) == 0 or len(test_name) == 0:
    error_happened()
else:
    open(test_dir+test_name+".in", "w").close()
    open(test_dir+test_name+".out", "w").close()
    open(test_dir+test_name+".err", "w").close()
    print(".err, .in and .out files created and made enpty.") 
    err_code = console_input("Enter error code:")
    # error code:
    try:
        code = int(err_code)
        text_file = open(test_dir+test_name+".code", "w")
        text_file.write(str(code))
        text_file.close()
    except ValueError:
        #Handle the exception
        print("Error code must be a number, that wasn't number.") 
    # arguments:
    arguments = console_input("Enter program arguments:")
    if len(arguments)>0:
        text_file = open(test_dir+test_name+".args", "w")
        text_file.write(arguments)
        text_file.close()
    timeout = console_input("Enter program timeout (ms):")
    # error code:
    try:
        code = int(timeout)
        text_file = open(test_dir+test_name+".time", "w")
        text_file.write(str(code))
        text_file.close()
    except ValueError:
        #Handle the exception
        print("Timeout must be a number, that wasn't number.") 


def error_happened():
    print("Usage:\n     python make_test.py TEST_DIR_PATH TEST_NAME")