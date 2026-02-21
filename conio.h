#ifndef CONIO_H
#define CONIO_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define cprintf printf
//#define cscanf scanf
 
#define CLEAR "\x1b[2J"
#define SET11 "\x1b[1;1f"
#define CURSOR_UP "\x1b[1A"
#define ERASE_LINE "\x1b[2K"
#define BLINK_SLOW "\x1b[5m"
#define BLINK_RAPID "\x1b[6m"
#define CC_CLEAR "\x1b[0m"

enum COLORS
{
  BLACK = 0,
  BLUE = 1,
  GREEN = 2,
  CYAN = 3,
  RED = 4,
  MAGENTA = 5,
  BROWN = 6,
  LIGHTGRAY = 7,
  DARKGRAY = 8,
  LIGHTBLUE = 9,
  LIGHTGREEN = 10,
  LIGHTCYAN = 11,
  LIGHTRED = 12,
  LIGHTMAGENTA = 13,
  YELLOW = 14,
  WHITE = 15,
  BLINK = 128
};

const char* get_mapped_char(unsigned char c) {
    switch(c) {
        case 176: return "░"; case 177: return "▒"; case 178: return "▓";
        case 179: return "│"; case 180: return "┤"; case 181: return "╡";
        case 182: return "╢"; case 183: return "╖"; case 184: return "╕";
        case 185: return "╣"; case 186: return "║"; case 187: return "╗";
        case 188: return "╝"; case 189: return "╜"; case 190: return "╛";
        case 191: return "┐"; case 192: return "└"; case 193: return "┴";
        case 194: return "┬"; case 195: return "├"; case 196: return "─";
        case 197: return "┼"; case 198: return "╞"; case 199: return "╟";
        case 200: return "╚"; case 201: return "╔"; case 202: return "╩";
        case 203: return "╦"; case 204: return "╠"; case 205: return "═";
        case 206: return "╬"; case 207: return "╧"; case 208: return "╨";
        case 209: return "╤"; case 210: return "╥"; case 211: return "╙";
        case 212: return "╘"; case 213: return "╒"; case 214: return "╓";
        case 215: return "╫"; case 216: return "╪"; case 217: return "┘";
        case 218: return "┌"; case 219: return "█"; case 220: return "▄";
        case 221: return "▌"; case 222: return "▐"; case 223: return "▀";
        case 254: return "■";
        default: return NULL;
    }
}

struct termios oldterm, newterm;
int unget_char = -1; // Buffer para simular o 224 do Windows
int is_termios_saved = 0; // Trava de segurança para o terminal

void resetTermios(void)
{
    // Só restaura se realmente tiver salvo o estado original antes
    if (is_termios_saved) {
        tcsetattr(0, TCSANOW, &oldterm);
    }
}

void initTermios(int echo)
{
    // Salva o estado original do terminal APENAS na primeira vez que rodar
    if (!is_termios_saved) {
        tcgetattr(0, &oldterm);
        is_termios_saved = 1;
        atexit(resetTermios); // Garante a restauração automática se o programa fechar ou crashar
    }
    
    newterm = oldterm;
    // Desativa modo canônico E os sinais do sistema        ISIG libera o Backspace e mapeia o Ctrl+C cru
    newterm.c_lflag &= ~(ICANON | ISIG); 
    newterm.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &newterm);
}

int getch_(int echo)
{
    if (unget_char != -1) {
        int ch = unget_char;
        unget_char = -1;
        return ch;
    }

    int ch;
    initTermios(echo);
    ch = getchar();

    // Lida com esc e teclas especiais
    if (ch == 27) { 
        int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        
        int seq1 = getchar();
        if (seq1 == '[') { 
            int seq2 = getchar();
            
            // Setas direcionais
            if (seq2 == 'A') { ch = 224; unget_char = 72; }      // Cima
            else if (seq2 == 'B') { ch = 224; unget_char = 80; } // Baixo
            else if (seq2 == 'C') { ch = 224; unget_char = 77; } // Direita
            else if (seq2 == 'D') { ch = 224; unget_char = 75; } // Esquerda
            // Teclas extras Delete, Home, End. que geram sequências com ~
            else if (seq2 >= '1' && seq2 <= '6') {
                int seq3 = getchar(); // Limpa o ~ final para não sujar o buffer
                if (seq2 == '3' && seq3 == '~') { ch = 224; unget_char = 83; } // Delete
            }
        } else if (seq1 != EOF) {
            ungetc(seq1, stdin); // Era só o esc sozinho mesmo
        }
        
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    } 
    else if (ch == 10) { 
        ch = 13; // Enter do Linux para Enter do Windows
    }
    else if (ch == 127) {
        ch = 8; // Backspace do Linux para Backspace do Windows (\b)
    }

    resetTermios();
    return ch;
}

void cagxy(unsigned int x, unsigned int y)
{
    printf("%s\x1b[%d;%df", CLEAR, y, x);
}

void clrscr()
{
    printf("%s%s",CLEAR, SET11);
}

int getch(void)
{
    return getch_(0);
}

int getche(void)
{
    return getch_(1);
}

void gotox(unsigned int x)
{
    printf("\x1b[%dG", x);
}

void gotoxy(unsigned int x, unsigned int y)
{
    printf("\x1b[%d;%df", y, x);
    fflush(stdout);
}

void nocursor()
{
    printf("\x1b[?25l");
}

void normvideo()
{
    printf("\x1b[0m");
}

void showcursor()
{
    printf("\x1b[?25h");
}

void textcolor(int newcolor)
{
  const char * s = "\x1b[30m";

  switch (newcolor)
  {
  case BLACK:        s = "\x1b[30m"; break;
  case BLUE:         s = "\x1b[34m"; break;
  case GREEN:        s = "\x1b[32m"; break;
  case CYAN:         s = "\x1b[36m"; break;
  case RED:          s = "\x1b[31m"; break;
  case MAGENTA:      s = "\x1b[35m"; break;
  case BROWN:        s = "\x1b[33m"; break;
  case LIGHTGRAY:    s = "\x1b[37m"; break;
  case DARKGRAY:     s = "\x1b[30;1m"; break;
  case LIGHTBLUE:    s = "\x1b[34;1m"; break;
  case LIGHTGREEN:   s = "\x1b[32;1m"; break; 
  case LIGHTCYAN:    s = "\x1b[36;1m"; break;
  case LIGHTRED:     s = "\x1b[31;1m"; break;
  case LIGHTMAGENTA: s = "\x1b[35;1m"; break;
  case YELLOW:       s = "\x1b[33;1m"; break;
  case WHITE:        s = "\x1b[37;1m"; break;
  case BLINK:        s = "\x1b[5m"; break;
  };

  printf("%s", s);
}

void textbackground(int newcolor)
{
  const char * s = "\x1b[40m"; // Padrão

  switch (newcolor)
  {
  case BLACK:        s = "\x1b[40m"; break;
  case BLUE:         s = "\x1b[44m"; break;
  case GREEN:        s = "\x1b[42m"; break;
  case CYAN:         s = "\x1b[46m"; break;
  case RED:          s = "\x1b[41m"; break;
  case MAGENTA:      s = "\x1b[45m"; break;
  case BROWN:        s = "\x1b[43m"; break;
  case LIGHTGRAY:    s = "\x1b[47m"; break;
  case DARKGRAY:     s = "\x1b[100m"; break;
  case LIGHTBLUE:    s = "\x1b[104m"; break;
  case LIGHTGREEN:   s = "\x1b[102m"; break;
  case LIGHTCYAN:    s = "\x1b[106m"; break;
  case LIGHTRED:     s = "\x1b[101m"; break;
  case LIGHTMAGENTA: s = "\x1b[105m"; break;
  case YELLOW:       s = "\x1b[103m"; break;
  case WHITE:        s = "\x1b[107m"; break;
  case BLINK:        s = "\x1b[40m"; break;
  };

  printf("%s", s);
}

void delline()
{
    printf("%s%s", ERASE_LINE, CURSOR_UP);
}

void clreol()
{
    printf("%s",CLEAR);
}

int putch(const char c)
{
    unsigned char uc = (unsigned char)c;
    const char *mapped = get_mapped_char(uc);
    if (mapped) fprintf(stdout, "%s", mapped);
    else putchar(uc);
    fflush(stdout);
    return (int)c;
}

int cputs(const char*str)
{
    printf("%s", str);
    return 0;
}

int wherexy(int *x, int *y)
{
    printf("\033[6n");
    if(getch() != '\x1B') return 0;
    if(getch() != '\x5B') return 0;
    int in;
    int ly = 0;
    while((in = getch()) != ';')
        ly = ly * 10 + in - '0';
    int lx = 0;
    while((in = getch()) != 'R')
        lx = lx * 10 + in - '0';
    *x = lx;
    *y = ly;
    return 1;
}

int wherex()
{
    int x=0,y=0;
    wherexy(&x, &y);
    return x;
}

int wherey()
{
    int x=0,y=0;
    wherexy(&x, &y);
    return y;
}

int kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO | ISIG);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if(ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

//implementando gets personalizado pois foi desativado no C11
char* conio_gets(char* str)
{
    int len = 0; 
    int pos = 0; 
    int ch;
    int continuar = 1;

    while (continuar) {
        ch = getch(); 

        if (ch == 13 || ch == 10) { // Enter
            str[len] = '\0';
            putch('\n');
            continuar = 0; 
        } 
        else if (ch == 3) { // Ctrl+C
            exit(0); 
        }
        else if (ch == 8 || ch == 127) { // Backspace
            if (pos > 0) {
                for (int j = pos; j < len; j++) {
                    str[j - 1] = str[j];
                }
                pos--;
                len--;
                str[len] = '\0';

                printf("\b"); 
                for (int j = pos; j < len; j++) putch(str[j]); 
                printf(" "); 
                
                for (int j = 0; j <= (len - pos); j++) printf("\b");
                fflush(stdout);
            }
        } 
        else if (ch == 224) { // Teclas de seta e Delete
            int seq = getch(); 
            if (seq == 75) { // Esquerda
                if (pos > 0) { pos--; printf("\b"); fflush(stdout); }
            } 
            else if (seq == 77) { // Direita
                if (pos < len) { putch(str[pos]); pos++; fflush(stdout); }
            }
            else if (seq == 83) { // Delete
                if (pos < len) {
                    for (int j = pos + 1; j < len; j++) str[j - 1] = str[j];
                    len--; str[len] = '\0';
                    for (int j = pos; j < len; j++) putch(str[j]);
                    printf(" "); 
                    for (int j = 0; j <= (len - pos); j++) printf("\b"); 
                    fflush(stdout);
                }
            }
        } 
        else if (ch >= 32 && ch <= 255) { // Caracteres imprimíveis
            for (int j = len; j > pos; j--) {
                str[j] = str[j - 1];
            }
            str[pos] = ch;
            len++;
            pos++;
            str[len] = '\0';

            putch(ch);
            for (int j = pos; j < len; j++) putch(str[j]);
            for (int j = 0; j < (len - pos); j++) printf("\b");
            fflush(stdout);
        }
    }

    return str;
}

int conio_printf(const char *format, ...) {
    char buf[4096];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);

    for (int i = 0; i < len; i++) {
        unsigned char uc = (unsigned char)buf[i];
        const char *mapped = get_mapped_char(uc);
        if (mapped) fprintf(stdout, "%s", mapped);
        else putchar(uc);
    }
    fflush(stdout);
    return len;
}

#undef printf
#define printf conio_printf
#undef cprintf
#define cprintf conio_printf
#undef gets
#define gets conio_gets

#endif