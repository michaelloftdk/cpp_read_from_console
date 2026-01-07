#ifndef GETCONSOLECHAR_HPP
#define GETCONSOLECHAR_HPP

#ifdef __WIN32
#define __WINDOWS__
#endif
#ifdef __WIN64
#define __WINDOWS__
#endif

#ifdef __WINDOWS__
#include <conio.h>
int _getCh()
{
    return _getch();
}
#else // Linux / MacOS

#include <stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

class LinuxGetchar {
  public:
    static int getChar()
    {
        static LinuxGetchar instance; // Singleton to manage terminal settings
        return instance.getCharInternal();
    }

  private:
    LinuxGetchar()
    {
        /*tcgetattr gets the parameters of the current terminal
        STDIN_FILENO will tell tcgetattr that it should write the settings
        of stdin to oldt*/
        tcgetattr( STDIN_FILENO, &oldt);

        /*now the settings will be copied*/
        newt = oldt;

        /*ICANON normally takes care that one line at a time will be processed
        that means it will return if it sees a "\n" or an EOF or an EOL*/
        newt.c_lflag &= ~(ICANON);

        newt.c_lflag &= ~(ECHO); //disable echo as well

        /*Those new settings will be set to STDIN
        TCSANOW tells tcsetattr to change attributes immediately. */
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
    }

    ~LinuxGetchar()
    {
        /*restore the old settings*/
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
    }

    struct termios oldt, newt;
    int getCharInternal()
    {
        return getchar();
    }
};

int _getch()
{
    return LinuxGetchar::getChar();
}

#endif // __WINDOWS__
#endif // GETCONSOLECHAR_HPP