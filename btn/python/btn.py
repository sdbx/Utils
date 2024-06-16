import sys
import string

def main(n):
    text = sys.stdin.read()
    if len(text) == 0:
        raise_err('nsy: EOF detected.')
    
    text = text.translate(str.maketrans('', '', string.whitespace))
    for i in range(0, len(text), n):
        print(text[i : i + n])

def raise_err(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)
    sys.exit(1)

if len(sys.argv) != 2:
    raise_err(
        'nsy: argc != 2.',
        'Usage : python btn.py N',
        '  where\n\tN = the number of characters for each line.',
        sep='\n')

n = int(sys.argv[1])
if n < 1 or n > 32:
    raise_err('nsy: argv[1] out of range.')

main(n)
