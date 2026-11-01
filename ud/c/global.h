#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef PROGNAME
#define EXECNAME PROGNAME
#else
#define EXECNAME "ud"
#endif

#define ENPREFIX EXECNAME ": "

#ifdef DIR_DATA
#define DIR_MANPAGE DIR_DATA "/man/man1/ud.1"
#else
#define DIR_MANPAGE "/usr/local/share/man/man1/ud.1"
#endif

#define DIR_MANPAGE_TXT DIR_MANPAGE ".txt"

#endif
