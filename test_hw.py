#!/usr/bin/env python

# Homework tester script
# ----------------------
# This script will automatically (try to) test 
# IO of your homework.
#
# Requirements:
#    gcc compiler, must be on system path
# 
# Parameters:
#    Drag'n'Drop: Drag your homework C file on this script. The file MUST have .c extension
#    Command line argument: pass path to your file as first argument. Eg. `python test_hw.py "~/PRP/HW02/main.c"
#    Manual input: This only works of homework is in current working directory, you will be prompted for it's name
# 
# Tests:
#    Tests must follow this rules:
#      - Every test input must have ".in" extension
#      - Every test output must have ".out" extension
#      - Every test standard error output must have ".err" extension
#      - Every required error code must be in ".code" file
#      - Parameters to pass to the program must be in ".args" file
#     All aforementioned files must share the same name. Eg "test.in", "test.out", "test.err" and "test.code".
#     Test input files are required to exist in order to start the test.
#     All other files are optional, if missing, output will not be checked.
#     Error code is always checked and must be 0 unless specified otherwise in .code file.
# 
# Test directory structure
#    Test directory must be in same folder as your `.c` file. The directory name
#    should be "TEST-[Name of the C file]".
#    That is, the structure should look like this:
#    
#      - TEST-main
#         - pub1.in
#         - pub1.out
#         - error-test.in
#         - error-test.out
#         - error-test.code
#      - main.c
# Results
#    The script will only output test names unless something goes wrong.
#    If there is something different in the output, python standard diff from difflib will be printed.

#python 2.7 vs python 3
import sys
if sys.version_info < (3, 0):
    def console_input(prompt):
        return raw_input(prompt)
    str_type = basestring
    sys.maxsize = sys.maxint
else:
    def console_input(prompt):
        return input(prompt)
    str_type = str

import glob
import re
import os.path
import sys

def file_or_none(filename, dir):
    if os.path.isfile(dir+filename):
        return filename
    else:
        return None
        
def clear_carriage_returns(text):
    return text.replace("\r", "").replace("\\r", "").replace("\\n", "\n")
# This handles top level exceptions so that the window doesn't close
def show_exception_and_exit(exc_type, exc_value, tb):
    import traceback
    traceback.print_exception(exc_type, exc_value, tb)
    console_input("Press key to exit.")
    sys.exit(-1)
sys.excepthook = show_exception_and_exit


name = "";
du_dir = "./"
execname = ""
if len(sys.argv) > 1:
    c_regex = re.compile("([a-zA-Z0-9_\-\./\\\\: ]+[\\\\/])?([a-zA-Z0-9_\-\.]+)\.(c|exe)$")
    matches = c_regex.match(sys.argv[1])
    #print(sys.argv)
    #print(matches)
    #print(matches.group(1))
    dirname = matches.group(1)
    hwname = matches.group(2)
    if dirname is not None and len(dirname) > 0:
        du_dir = dirname
    if hwname is None or len(hwname) == 0:
        raise Exception("Cannot find homework name in path. Try using ./homework.c or ./homework.exe")
    name = hwname
    if matches.group(3)=="exe":
        execname = path.join(du_dir, name)
# Get name of the HW and list of innuts
if len(name) == 0:
    name = console_input("Enter HW name (without extension, eg \"HW02\" or \"main\"):")

testdir = du_dir+"./TEST-"+name+"/"
inputs = glob.glob(testdir+"*.in")
c_file_path = du_dir + name + ".c"

stdout = []
stderr = []
return_codes = []
argslists = []
timeouts = []
# Helper array to remember names of individual inputs
names = []
filename_regex = re.compile("[/\\\\](([^\\\\/]+)\.in)$")
# Find outputs for inputs
for index in range(len(inputs)):
    matches = filename_regex.search(inputs[index])
    inputs[index] = matches.group(1)
    case_name = matches.group(2);
    out = case_name+".out"
    stdout.append(file_or_none(out, testdir))
    err = case_name+".err"
    stderr.append(file_or_none(err, testdir))
    
    retcode = file_or_none(case_name+".code", testdir)
    if retcode is None:
        return_codes.append(0)
    else:
        return_codes.append(int(open(testdir+retcode, 'r').read()))

    args = file_or_none(case_name+".args", testdir)
    if args is None:
        argslists.append([])
    else:
        argslists.append(open(testdir+args, 'r').read().split(" "))

    timeout = file_or_none(case_name+".time", testdir)
    timeout_no = sys.maxsize
    if timeout is not None:
        try:
            time = open(testdir+timeout, 'r').read()
            timeout_no = int(time)
        except ValueError:
            #Handle the exception
            print("ERROR in "+testdir+case_name+".time - Timeout must be a number, that wasn't number.") 
    timeouts.append(timeout_no)

    names.append(case_name)

if execname=="" or len(execname)==0:
    # Compile the homework assignment using GCC
    from subprocess import Popen, PIPE
    compile_args = ["gcc", "-Wall","-pedantic", "-std=c99", c_file_path, "-o"+du_dir+name]
    process = Popen(compile_args)
    return_code = process.wait()
    if return_code != 0:
        raise Exception("Error during compilation... sorry.")
    execname = du_dir+name

# The actual testing here
# we start a process for every file, capture output and save it in output file
# if possible, we compare the output with required output
from DiffStuff import print_diff

import platform, threading
import time
if platform.system() == "Windows" and not execname.endswith(".exe"):
    execname = execname + ".exe"
print("Running tests on "+execname + "")
for index in range(len(inputs)):
    casename = names[index]
    print("Test case: "+ casename)
    sys.stdout.flush()
    # Load test input from file
    myinput = open(testdir + inputs[index])
    # load args, if any
    time_start = time.clock()
    process = Popen([execname]+argslists[index], stdin=myinput, stdout=PIPE, stderr=PIPE)
    (out, err) = process.communicate()
    exit_code = process.wait()
    time_end = time.clock()
    
    if not isinstance(out, str_type):
        out = str(out.decode('ascii'))
    if not isinstance(err, str_type):
        err = str(err.decode('ascii'))
    out = clear_carriage_returns(out)
    err = clear_carriage_returns(err)
    rq_code = return_codes[index]
    rq_out = None
    if stdout[index] is not None:
        rq_out = clear_carriage_returns(open(testdir+stdout[index], 'r').read())
    rq_err = None
    if stderr[index] is not None:
        rq_err = clear_carriage_returns(open(testdir+stderr[index], 'r').read())
    # print("Required output: "+rq_out)
    problem = False
    
    if exit_code!=rq_code:
        print("  ERROR: Return code: "+str(exit_code)+" does not match required code "+str(rq_code))
        problem = True
    if (rq_out is not None) and rq_out!=out:
        print("  ERROR: Output does not match required output. (note that empty file is also kind of required output).")
        print("  DIFF:\n")
        print_diff(out, rq_out)
        problem = True
    if (rq_err is not None) and rq_err!=err:
        print("  ERROR: Error output does not match required output.")
        if err is None:
            print("    ... there was no error output at all.\n")
        else:
            print("  DIFF:\n")
            print_diff(err, rq_err)
        problem = True
    print("\n  Duration: "+str(time_end-time_start)+" ms")
    if problem:
        print("    ERROR!!!\n")
    else:
        print("    ...OK\n")
    print("\n----------------------------\n")
        
console_input("Press enter to quit");