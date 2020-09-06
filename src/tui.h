/*
 * 'textual user interface'
 *
 * Author : P.J. Kunst <kunst@prl.philips.nl>
 * Date   : 1993-02-25
 */

#ifndef _TUI_H_
#define _TUI_H_

#include <curses.h>

#include <iostream>
#include <string.h>

#ifdef A_COLOR
#define A_ATTR  (A_ATTRIBUTES ^ A_COLOR)  /* A_BLINK, A_REVERSE, A_BOLD */
#else
#define A_ATTR  (A_ATTRIBUTES)            /* standard UNIX attributes */
#endif

#define MAXSTRLEN  256
#define KEY_ESC    0x1b     /* Escape */

// vs2015 and above has function snprintf - add by itas109 2020-09-06
#if defined(_MSC_VER) && _MSC_VER < 1900
int  vsnprintf(char *outBuf, size_t size, const char *format, va_list ap)
{
    int count = -1;
 
    if (size != 0)
    {
        count = _vsnprintf_s(outBuf, size, _TRUNCATE, format, ap);
    }
        
    if (count == -1)
    {
        count = _vscprintf(format, ap);
    }
 
    return count;
}

int snprintf(char *outBuf, size_t size, const char *format, ...)
{
    int count;
    va_list ap;

    va_start(ap, format);
    count = vsnprintf(outBuf, size, format, ap);
    va_end(ap);
 
    return count;
}
#endif

typedef void (*FUNC)(void);

typedef struct
{
    char *name; /* item label */
    FUNC  func; /* (pointer to) function */
    char *desc; /* function description */
} menu;

/* ANSI C function prototypes: */

void    clsbody(void);
int     bodylen(void);
WINDOW *bodywin(void);

void    rmerror(void);
void    rminfo(void); // itas109 2020-09-05
void    rmstatus(void);

void    titlemsg(char *msg);
void    bodymsg(char *msg);
void    errormsg(char *msg);
void    infomsg(char *msg); // itas109 2020-09-05
void    statusmsg(char *msg);

bool    keypressed(void);
int     getkey(void);
int     waitforkey(void);

void    DoExit(void);
void    startmenu(menu *mp, char *title);
void    domenu(menu *mp);

int     weditstr(WINDOW *win, char *buf, int field);
WINDOW *winputbox(WINDOW *win, int nlines, int ncols);
int     getstrings(char *desc[], char *buf[], int field);

#define editstr(s,f)           (weditstr(stdscr,s,f))
#define mveditstr(y,x,s,f)     (move(y,x)==ERR?ERR:editstr(s,f))
#define mvweditstr(w,y,x,s,f)  (wmove(w,y,x)==ERR?ERR:weditstr(w,s,f))

#define inputbox(l,c)          (winputbox(stdscr,l,c))
#define mvinputbox(y,x,l,c)    (move(y,x)==ERR?w:inputbox(l,c))
#define mvwinputbox(w,y,x,l,c) (wmove(w,y,x)==ERR?w:winputbox(w,l,c))

#endif
