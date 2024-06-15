import sys

def main(n):
    text = input().replace(" ", "")
    if len(text) == 0:
        print("nsy: EOF detected.")
        sys.exit(1)
    for i in range(0, len(text), n):
        print(text[i : i + n])

if len(sys.argv) == 1:
    print("nsy: argc != 2.")
    print("Usage : python btn.py N")
    print("  where\n\tN = the number of characters for each line.")
    sys.exit(1)
n = int(sys.argv[1])
if n < 1 or n > 32:
    print("nsy: argv[1] out of range.")
    sys.exit(1)
main(n)
