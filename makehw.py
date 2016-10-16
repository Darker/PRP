import re

h = open('HWXX.h', 'r').read()
c = open('HWXX.c', 'r').read()

name = raw_input("Enter HW number:")
name = "HW"+name;

open(name+'.h', 'w').write(h.replace("HWXX", name));
open(name+'.c', 'w').write(c.replace("HWXX", name));