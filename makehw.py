import re
import sys
if sys.version_info < (3, 0):
    def console_input(prompt):
        return raw_input(prompt)
else:
    def console_input(prompt):
        return input(prompt)
h = open('HWXX.h', 'r').read()
c = open('HWXX.c', 'r').read()

name = console_input("Enter HW number:")
name = "HW"+name;

open(name+'.h', 'w').write(h.replace("HWXX", name));
open(name+'.c', 'w').write(c.replace("HWXX", name));