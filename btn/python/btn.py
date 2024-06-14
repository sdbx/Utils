import sys

def main(n):
    try:
        text = input().replace(" ", "")
        if len(text) == 0:
            raise Exception("nsy: EOF detected.")
        for i in range(0, len(text), n):
            print(text[i : i + n])
    except Exception as e:
        print(e)

try:
    if len(sys.argv) == 1:
        raise IndexError("nsy: argc != 2.")
    n = int(sys.argv[1])
    if n < 1 or n > 32:
        raise Exception("nsy: argv[1] out of range.")
    main(n)
except Exception as e:
    print(e)
    if type(e).__name__ == "IndexError":
        print("Usage : python btn.py N")
        print("  where\n\tN = the number of characters for each line.")
    sys.exit()