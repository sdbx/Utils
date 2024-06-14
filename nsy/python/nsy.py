import sys

def main(mark_kind):
    try:
        text = input().replace(" ", "")
        if len(text) == 0:
            raise Exception("nsy: EOF detected.")
    
        if mark_kind == "e":
            print('',*text,'',sep='！')
        elif mark_kind == "q":
            print('',*text,'',sep='？')
        elif mark_kind == "eu":
            print('',*text,'',sep=' ¡ ')
        elif mark_kind == "qu":
            print('',*text,'',sep=' ¿ ')
        else:
            raise Exception("nsy: unknown delimiter.")
    except Exception as e:
        print(e)

try:
    if len(sys.argv) == 1:
        raise IndexError("nsy: argc != 2.")
    mark_kind = sys.argv[1]
    main(mark_kind)
except Exception as e:
    print(e)
    if type(e).__name__ == "IndexError":
        print("Usage : python nsy.py MARK_KIND")
        print("  where\n\tMARK_KIND = e | q | eu | qu")
    sys.exit()