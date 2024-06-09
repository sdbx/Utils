import sys

def main(n, filename):
    with open(filename, "r") as f:
        text = "".join(f.readlines()).replace(" ", "")

    for i in range(0, len(text), n):
        print(text[i : i + n])

try:
    n = int(sys.argv[1])
    filename = sys.argv[2]
    main(n, filename)
except Exception as e:
    print(e)
    print("Usage : btn.py [N] [FILENAME]")
    print("N : number of characters for each line")
    sys.exit()