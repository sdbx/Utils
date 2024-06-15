import sys

def main(mark_kind):
    text = input().replace(" ", "")
    if len(text) == 0:
        print("nsy: EOF detected.")
        return
    match mark_kind:
        case "e":
            print('',*text,'',sep='！')
        case "q":
            print('',*text,'',sep='？')
        case "eu":
            print('',*text,'',sep=' ¡ ')
        case "qu":
            print('',*text,'',sep=' ¿ ')
        case _:
            print("nsy: unknown delimiter.")

if len(sys.argv) == 1:
    print("nsy: argc != 2.")
    print("Usage : python nsy.py MARK_KIND")
    print("  where\n\tMARK_KIND = e | q | eu | qu")
    sys.exit(1)
    
mark_kind = sys.argv[1]
main(mark_kind)
