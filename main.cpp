#include "GetConsoleChar.hpp"

int main(void){
    int c;

    while((c=_getch())!= 'e')
        putchar(c);

    return 0;
}