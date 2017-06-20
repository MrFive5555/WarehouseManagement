#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

#include <windows.h>

#include "screen.h"
#include "fileClass.h"

screenClass::screenClass()
{
    page.push_back({});
    nowPage = 0;
    nowLine = 0;
    adminfile.open("data/admin.data", fstream::out | fstream::ate | fstream::in);
    if (!adminfile) {
        adminfile.open("data/admin.data", fstream::in | fstream::out | fstream::ate | fstream::trunc);
    }
}

void screenClass::gotoXY(short line, short row) //goto the line x, colume y
{ 
    COORD point{ OFFSET_ROW + row, OFFSET_LINE + line };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
    nowLine = OFFSET_LINE + line;
}

void screenClass::gotoCommandLine()
{
    gotoXY(SCREEN_HIGHT - 1, 1);
}

void screenClass::newLine()
{
    gotoXY(nowLine + 1, 1);
}

void screenClass::beginOfLine()
{
    gotoXY(nowLine, 1);
}

void screenClass::printMessage(string message)
{
    clearMessage();
    gotoXY(SCREEN_HIGHT - 2, 1);
    cout << message;
    gotoCommandLine();
}

void screenClass::printScreen() {
    gotoXY(0, 0);
    for (int i = 0; i < SCREEN_LONG; ++i) {
        cout << '*';
    }

    for (int i = 0; i < SCREEN_HIGHT; ++i) {
        gotoXY(i, 0);
        cout << '*';
        gotoXY(i, SCREEN_LONG);
        cout << '*';
    }

    gotoXY(SCREEN_HIGHT, 0);
    for (int i = 0; i <= SCREEN_LONG; ++i) {
        cout << '*';
    }

    gotoXY(SCREEN_HIGHT - 3, 0);
    for (int i = 0; i <= SCREEN_LONG; ++i) {
        cout << '*';
    }

    gotoCommandLine();
}

void screenClass::clearCommandLine()
{
    gotoCommandLine();
    for (int i = 0; i != SCREEN_LONG - 2; ++i) {
        cout << " ";
    }
    gotoCommandLine();
}

void screenClass::clearMessage()
{
    gotoXY(SCREEN_HIGHT - 2, 1);
    for (int i = 0; i != SCREEN_LONG - 2; ++i) {
        cout << " ";
    }
    gotoXY(1, 1);
}

void screenClass::clearScreen()
{
    gotoXY(1, 1);
    string empty(SCREEN_LONG - 1, ' ');
    for (int i = 1; i != SCREEN_HIGHT - 3; ++i) {
        cout << empty;
        newLine();
    }
}

void screenClass::printHelp()
{
    clearScreen();
    gotoXY(1, 1); 
    cout << " <help>"; newLine();
    cout << "   type the command below to manage the warehouse"; newLine();
    newLine();
    cout << "   add + goods name + count : "; newLine();
    cout << "       add the goods in warehouse£¬e.g. add milk 30"; newLine();
    cout << "   del + goods name + count : "; newLine();
    cout << "       delete the goods from the warehouse, e.g. del milk 20"; newLine();
    /*cout << "   up :"; newLine(); 
    cout << "   down :"; newLine();
    cout << "       when the information on the screen is too much, "; newLine();
    cout << "       you can use up and down to make page up and down"; newLine();*/
    cout << "   ls :"; newLine();
    cout << "       list all goods in the warehouse"; newLine();
    cout << "   ls + goods name :"; newLine();
    cout << "       list the goods information"; newLine();
    cout << "   his :"; newLine();
    cout << "       list all operate history"; newLine();
    cout << "   his + goods name :"; newLine();
    cout << "       list the history about goods"; newLine();
    cout << "   clear :"; newLine();
    cout << "       clear the screen"; newLine();
    cout << "   logout : "; newLine();
    cout << "       log out and change administartor"; newLine();
    cout << "   help :"; newLine();
    cout << "       change the screen to the help page"; newLine();
    cout << "   quit :"; newLine();
    cout << "       quit the program"; newLine();

    gotoXY(SCREEN_HIGHT - 4, SCREEN_LONG - 2 - static_cast<short>(strlen("<type \"ret\" to continue>")));
    //Be careful : type convert from size_t to short many lose data!!!

    cout << "<type \"ret\" to continue>";
}

bool screenClass::LogIn()
{
    begin:
    gotoXY(SCREEN_HIGHT / 3, SCREEN_LONG / 3 - 15 );
    cout << "Warehouse Management System";
    gotoXY(SCREEN_HIGHT / 3 + 7, 30);
    cout << "do you have account now ? ";
    gotoXY(SCREEN_HIGHT / 3 + 9, 30);
    cout << "if yes, type \"login\", else type \"register\"";
    gotoCommandLine();
    string command;
    cin >> command;
    clearCommandLine();

    while (command != "login" && command != "register") {
        printMessage("please enter \"login\" or \"register\"");
        clearCommandLine();
        gotoCommandLine();
        cin >> command;
        clearCommandLine();
    }

    if (command == "login") {
        adminName = getAdminName();
    } else if (command == "register") {
        registerAdmin();
        printMessage("register successfully, please enter \"login\" or \"register\"");
        goto begin;
    }

    return true;
}

void screenClass::LogOut()
{
    clearScreen();
    clearMessage();
    clearCommandLine();
    LogIn();
    clearScreen();
}

void screenClass::leave()
{
    gotoXY(SCREEN_HIGHT + 1, 0);
}

bool screenClass::getCommand(fileClass& goods) {
    static string lastCom;
    gotoCommandLine();
    string line;
    getline(cin, line);
    stringstream comStr(line); //command string stream
    string command;

    comStr >> command;

    if (command == "add") {
        string name;
        int count = -1;

        comStr >> name >> count;

        if (count == -1) {
            printMessage("format error, format : add + goods name + count");
        } else if (count < 0) {
            printMessage("please enter the correct number");
        } else {
            //enter
            fileClass::STATE writeState = goods.writeRecord(name, count, adminName);
            //end enter
            stringstream outMessage;
            string mess;
            outMessage << "add successfully : " << name << ' ' << count;
            getline(outMessage, mess);
            printMessage(mess);
        }
        clearCommandLine();
    } else if (command == "del") {
        string name;
        int count = -1;

        comStr >> name >> count;

        if (count == -1) {
            printMessage("format error, format : del + goods name + count");
        } else if (count < 0) {
            printMessage("please enter the correct number");
        } else {
            stringstream outMessage;
            string mess;

            //delete
            fileClass::STATE delState = goods.writeRecord(name, -count, adminName);

            if (delState == fileClass::SUCCESS) {
                outMessage << "delete successfully : " << name << ' ' << count;
            } else if (delState == fileClass::NO_ENOUGH_GOODS) {
                outMessage << "delete unsuccessfully : no enough goods : " << name;
            } else if (delState == fileClass::NO_THIS_GOODS) {
                outMessage << "delete unsuccessfully : no this goods : " << name;
            } else {
                outMessage << "unknown error : you can try to restart the programe";
            }

            getline(outMessage, mess);
            printMessage(mess);
        }
        clearCommandLine();
    } else if (command == "ls") {
        string name;
        comStr >> name;
        if (name != "") {  
            fstream::pos_type pos = goods.getRecordPosition(name);
            if (pos != NO_RECORD) { //there is record
                clearPage();
                if (writeOneRecordInPage(goods, name)) { //there is record
                    printPage();
                } else { //no record
                    printMessage("there is no record in the warehouse");
                }
            } else { //no record
                printMessage("there is no record in the warehouse");
            }
        } else { //list all
            writeAllRecordInPage(goods);
            printPage();
            printMessage("list all");
        }
        clearCommandLine();
    } else if (command == "quit") {
        printMessage("quit");
        clearCommandLine();
        return false;
    } else if (command == "his") {
        string name;
        comStr >> name;
        if (name != "") {  
            fstream::pos_type pos = goods.getRecordPosition(name);
            if (pos != NO_RECORD) { //there is record
                clearPage();
                writeOneHistoryInPage(goods, name);
                printPage();
            } else { //no record
                printMessage("there is no record in the warehouse");
            }
        } else { //list all
            writeAllHistoryInPage(goods);
            printPage();
            printMessage("list all history");
        }
        clearCommandLine();
    } else if (command == "up") {
        if (!upPage()) {
            printMessage("it's the first page");
        }
    } else if (command == "down") {
        if (!downPage()) {
            printMessage("it's the lastest page");
        }
    } else if (command == "help") {
        printHelp();
    } else if (command == "clear") {
        //from (1, 1) to (SCREEN_LONG - 1, SCREEN_HIGHT - 4)
        clearPage();
        clearScreen();
    } else if (command == "ret") {
        printPage();
    } else if (command == "logout") {
        clearPage();
        LogOut();
        printHelp();
    } else if (command == "") {
        //do nothing
    } else {
        printMessage("no this command");
    }
    clearCommandLine();

    return true;
}

void screenClass::clearPage()
{
    page.erase(page.begin(), page.end());
    page.push_back({});
    nowPage = 0;
}

bool screenClass::upPage()
{
    if (nowPage == 0) {
        return false;
    } else if (nowPage - 1 >= 0) {
        --nowPage;
        printPage();
        return true;
    } else {
        return false;
    }
}

bool screenClass::downPage()
{
    if (nowPage + 1 < page.size()) {
        ++nowPage;
        printPage();
        return true;
    } else {
        return false;
    }
}

void screenClass::printPage()
{
    clearScreen();
    gotoXY(1, 1);

    //the blank range is from line 1 to SCREEN_HIGHT - 4
    //the last line are used as message
    screenClass::Page::const_iterator iter = page[nowPage].begin();
    for (int i = 1; i != SCREEN_HIGHT - 3; ++i) {
        if (iter != page[nowPage].end()) {
            beginOfLine();
            cout << *iter;
            ++iter;
        }
        newLine();
    }

    gotoXY(SCREEN_HIGHT - 4, 1);
    cout << "<type \"up\" or \"down\" to uppage or downpage>";
}

//the blank range is in the retangle from (1, 1) to (SCREEN_HIGHT-4, SCREEN_LONG-1)
//we leave the last line for message "type up or down to uppage or downpage"
//write the string content in the page now
void screenClass::writeInPage(string content, InfoType type)
{
    static screenClass::Page::size_type top = 0;
    string LIST_HEADER = (type == GOODS ? GOODS_HEADER : HIS_HEADER);
    if (page.size() == 0) { //if there is no page
        page.push_back({});
    }
    if (page[page.size() - 1].size() == 0) {  //the first line should be list header
                                              //the page[page.size()-1] is the lastest page
        page[page.size() - 1].push_back(LIST_HEADER);
    }
    if (page[page.size() - 1].size() == SCREEN_HIGHT - 5) {  //the screen is full
        page.push_back({}); //creat new page
        page[page.size() - 1].push_back(LIST_HEADER); // insert the list header
    }
    page[page.size() - 1].push_back(content);
}

//write the record of "name" good to page
//if there no record, it return false
bool screenClass::writeOneRecordInPage(fileClass& goods, string name)
{
    fstream::pos_type pos = goods.getRecordPosition(name);
    if (pos != NO_RECORD) {
        int count = 0;
        fstream::pos_type HisPos = 0;
        string time, lastAdmin;
        goods.getRecord(pos, name, count, HisPos, time, lastAdmin);

        if (count == 0) { //don't display a goods if it has no stock
            return false;
        }

        stringstream recStream; //record string stream
        recStream 
            << setw(17) << name
            << setw(16) << count
            << setw(26) << time
            << setw(26) << lastAdmin;
        string recString; //record string
        getline(recStream, recString);
        writeInPage(recString, GOODS);
        return true;
    }
    return false;
}

void screenClass::writeAllRecordInPage(fileClass& goods)
{
    clearPage();
    
    string name;
    int i = 1;

    goods.pointFile.seekg(i*POINTER_LENGTH);
    goods.pointFile >> name;
    while (!name.empty()) {
        writeOneRecordInPage(goods, name);
        goods.pointFile.seekg((++i)*POINTER_LENGTH);
        name = "";
        goods.pointFile >> name;
    }

    goods.fresh();
}

//if there is no record, the function will return false
void screenClass::writeOneHistoryInPage(fileClass& goods, string name)
{
    clearPage();
    History his;

    his.nextPos = goods.getHistoryPosition(name);

    while (his.nextPos != NO_RECORD) {
        his = goods.getHistory(his.nextPos);
        stringstream recStream;
        recStream 
            << setw(18) << his.goodsName
            << setw(12) << his.changeNumber
            << setw(14) << his.stock
            << setw(18) << his.adminName
            << setw(23) << his.time;
        string recStr;
        getline(recStream, recStr);
        writeInPage(recStr, HIS);
    }
}

void screenClass::writeAllHistoryInPage(fileClass& goods)
{
    clearPage();

    for (fstream::pos_type pos = HISTORY_LENGTH;
        pos != goods.fileEnd(goods.hisFile);
        pos += HISTORY_LENGTH) {

        History his = goods.getHistory(pos);
        stringstream recStream;
        recStream
            << setw(18) << his.goodsName
            << setw(12) << his.changeNumber
            << setw(14) << his.stock
            << setw(18) << his.adminName
            << setw(23) << his.time;
        string recStr;
        getline(recStream, recStr);
        writeInPage(recStr, HIS);
    }

    goods.fresh();
}

string screenClass::getAdminName()
{

    printMessage("please enter your account name : ");

    while (true) {
        gotoCommandLine();
        string name, password;
        cin >> name;
        clearCommandLine();
        printMessage("please enter your password : ");
        gotoCommandLine();
        cin >> password;
        clearCommandLine();

        adminfile.seekg(0, fstream::beg);
        string tempName, tempPassword;
        while (adminfile >> tempName >> tempPassword) {
            if (tempName == name && tempPassword == password) {
                return name;
            } else if (tempName == name && tempPassword != password) {
                break;
            }
        }

        if (tempName == name && tempPassword != password) {
            printMessage("password error, please enter your account again");
        } else {
            printMessage("no this account, please enter your account again");
        }
        adminfile.clear();
    }
}

void screenClass::registerAdmin()
{
    string name, password;
    printMessage("please enter your account you want : ");
    gotoCommandLine();
    cin >> name;
    clearCommandLine();

    while (name.length() > 15) {
        printMessage("your name is too long, please choice a new name");
        cin >> name;
        clearCommandLine();
    }

    while (true) {
        string tempName, tempPassword;
        adminfile.seekg(0, fstream::beg);
        while (adminfile >> tempName >> tempPassword) {
            if (tempName == name) {
                printMessage("there is a same account, please choice a new name : ");
                adminfile.clear();
                break;
            }
        }

        if (adminfile.eof()) {
            adminfile.clear();
            break;
        }
        
        cin >> name;
        clearCommandLine();
    }

    printMessage("please set your password : ");
    gotoCommandLine();
    cin >> password;
    adminfile << name << ' ' << password << endl;
    clearCommandLine();
    clearMessage();
}