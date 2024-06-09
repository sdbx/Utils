import sys

def main(filename):
    with open(filename, "r") as f:
        text = "".join(f.readlines()).replace(" ", "")
    print("！".join(["", *text]))

try:
    filename = sys.argv[1]
    main(filename)
except Exception as e:
    print(e)
    print("Usage : python nsy.py [FILE]")
    print("FILE : location of file which contains string")
    sys.exit()