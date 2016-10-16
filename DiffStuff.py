from difflib import Differ
from pprint import pprint
import sys

def readable_whitespace(line):
	#return line.replace("\n", "\\n\n")
	end = len(line.rstrip('\r\n'))
	return line[:end] + repr(line[end:])[1:-1] + '\n'

def print_diff(text1, text2):
	d = Differ()
	text1 = text1.splitlines(True)
	text2 = text2.splitlines(True)
	
	text1 = [readable_whitespace(line) for line in text1]
	text2 = [readable_whitespace(line) for line in text2]
	
	result = list(d.compare(text1, text2))
	sys.stdout.writelines(result)
	sys.stdout.write("\n")
	
if __name__ == "__main__":
	A = "AAABAAA\n"
	A += "BBB\n"
	B = "AAAAAAA\n"
	B += "\n"
	B += "BBB"
	print_diff(A,B)