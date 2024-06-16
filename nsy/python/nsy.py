import sys
import string

def main(mark_kind):
    text = sys.stdin.read()
    if len(text) == 0:
        raise_err('nsy: EOF detected.')
    
    text = text.translate(str.maketrans('', '', string.whitespace))
    match mark_kind:
        case 'e':
            print('',*text,'',sep='！')
        case 'q':
            print('',*text,'',sep='？')
        case 'eu':
            print('',*text,'',sep=' ¡ ')
        case 'qu':
            print('',*text,'',sep=' ¿ ')
        case _:
            raise_err('nsy: an unknown delimiter.')

def raise_err(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    sys.exit(1)

if len(sys.argv) != 2:
    raise_err(
        'nsy: argc != 2.',
        'Usage : python nsy.py MARK_KIND',
        '  where\n\tMARK_KIND = e | q | eu | qu',
        sep='\n')
    
mark_kind = sys.argv[1]
main(mark_kind)
