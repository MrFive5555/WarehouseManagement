#include "complieOption.h"
#include "fileClass.h"
#include "screen.h"

#ifdef MAIN

int main()
{
    screenClass screen;
    fileClass goods;

    screen.printScreen();

    while (!screen.LogIn());

    screen.printHelp();

    while (screen.getCommand(goods));

    screen.leave();
}

#endif // MAIN