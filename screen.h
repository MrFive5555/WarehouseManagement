#ifndef SCREEN_H_

#include <string>
#include <vector>
#include "fileClass.h"

#define OFFSET_ROW 13
#define OFFSET_LINE 0
#define SCREEN_LONG (90+2)
#define SCREEN_HIGHT (26+2)

using namespace std;

//the blank range is in the retangle from (1, 1) to (SCREEN_HIGHT-4, SCREEN_LONG-1)

class screenClass {
public:
    typedef vector<string> Page;

    typedef enum {
        GOODS,
        HIS
    } InfoType;

    screenClass();
    void printScreen();
    bool LogIn();
    void printHelp();
    bool getCommand(fileClass& goods);
    void leave();

private:
    void writeInPage(string content, InfoType type);
    bool writeOneRecordInPage(fileClass& goods, string name);
    void writeAllRecordInPage(fileClass& goods);

    void writeOneHistoryInPage(fileClass& goods, string name);
    void writeAllHistoryInPage(fileClass& goods);

    void gotoXY(short x, short y);
    void gotoCommandLine();
    void newLine();
    void beginOfLine();

    void printPage();
    void printMessage(string message);

    void LogOut();
    string getAdminName();
    void registerAdmin();
    
    void clearCommandLine();
    void clearScreen();
    void clearMessage();

    bool upPage();
    bool downPage();
    
    void clearPage();

    string adminName;
    vector<screenClass::Page> page;
    size_t nowPage;
    int nowLine;
    const string GOODS_HEADER = "       goods name           count           last changed at                changed by ";
    const string HIS_HEADER =   "        goods name       count         stock        changed at             changed by ";
    fstream adminfile;
};

#endif // !SCREEN_H_

