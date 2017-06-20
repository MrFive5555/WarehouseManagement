#include "complieOption.h"
#include "screen.h"
#include "fileClass.h"

#ifdef NEWFILE1
int main()
{
    fileClass goods;

    if (!(goods.writeRecord("phone", 15, "five")==fileClass::SUCCESS )) {
        cout << "error1" << endl;
    }
    if (!(goods.writeRecord("milk", 23, "cloud") == fileClass::SUCCESS)) {
        cout << "error2" << endl;
    }
    if (!(goods.writeRecord("phone", 32, "six") == fileClass::SUCCESS)) {
        cout << "error3" << endl;
    }
    if (!(goods.writeRecord("phone", -3, "six") == fileClass::SUCCESS)) {
        cout << "error4" << endl;
    }
}
#endif // NEWFILE1

#ifdef  NEWFILE2
int main()
{
    fileClass goods;

    if (!(goods.writeRecord("phone", 15, "five") == fileClass::SUCCESS &&
        goods.writeRecord("milk", 23, "cloud") == fileClass::SUCCESS &&
        goods.writeRecord("apple", 32, "six") == fileClass::SUCCESS &&
        goods.writeRecord("beef", 12, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("car", 64, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("robet", 93, "Mike") == fileClass::SUCCESS &&
        goods.writeRecord("bowl", 5, "Smith") == fileClass::SUCCESS &&
        goods.writeRecord("shirt", 247, "Green") == fileClass::SUCCESS &&
        goods.writeRecord("robet", 93, "Green") == fileClass::SUCCESS &&
        goods.writeRecord("oil", 357, "five") == fileClass::SUCCESS &&
        goods.writeRecord("peanut", 256, "Smith") == fileClass::SUCCESS &&
        goods.writeRecord("watermelon", 124, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("paper", 293, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("rice", 53, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("book", 92, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("pen", 30, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("firework", 46, "Maxwell") == fileClass::SUCCESS &&
        goods.writeRecord("disk", 673, "Maxwell") == fileClass::SUCCESS &&
        goods.writeRecord("knife", 13, "Lucky") == fileClass::SUCCESS &&
        goods.writeRecord("umbrella", 4, "Daming") == fileClass::SUCCESS &&
        goods.writeRecord("TV", 24, "Han-Mei-Mei") == fileClass::SUCCESS &&
        goods.writeRecord("tea", 1267, "Tom") == fileClass::SUCCESS &&
        goods.writeRecord("monkey-module", 53, "Hill") == fileClass::SUCCESS &&
        goods.writeRecord("pen", 3, "Higs") == fileClass::SUCCESS &&
        goods.writeRecord("dry-ice", 1, "Lucky") == fileClass::SUCCESS &&
        goods.writeRecord("sodium-hydroxide", 4561, "YourFather") == fileClass::SUCCESS &&
        goods.writeRecord("ofo", 235, "Smith") == fileClass::SUCCESS &&
        goods.writeRecord("engine", 25, "Foxes") == fileClass::SUCCESS &&
        goods.writeRecord("Optimus-Prime", 73, "Pedestrian") == fileClass::SUCCESS)) {
        cout << "error" << endl;
    }
}
#endif //  NEWFILE2

#ifdef HIS1
int main()
{
    fileClass goods;
    History his;

    his.nextPos = goods.getHistoryPosition("phone");
    //cout << his.nextPos << endl;
    while (his.nextPos != NO_RECORD) {
        his = goods.getHistory(his.nextPos);
        cout << his.goodsName << " " << his.changeNumber << " " << his.stock << " " << his.adminName << " " << his.time << endl;
    }

    his.nextPos = goods.getHistoryPosition("milk");
    //cout << his.nextPos << endl;
    while (his.nextPos != NO_RECORD) {
        his = goods.getHistory(his.nextPos);
        cout << his.goodsName << " " << his.changeNumber << " " << his.stock << " " << his.adminName << " " << his.time << endl;
    }

    his.nextPos = goods.getHistoryPosition("sss");
    //cout << his.nextPos << endl;
    while (his.nextPos != NO_RECORD) {
        his = goods.getHistory(his.nextPos);
        cout << his.goodsName << " " << his.changeNumber << " " << his.stock << " " << his.adminName << " " << his.time << endl;
    }
    
}
#endif // HIS1

#ifdef WRITEPAGE1
int main()
{
    screenClass screen;
    fileClass goods;

    goods.writeRecord("phone", 15, "five");
    goods.writeRecord("milk", 23, "cloud");
    goods.writeRecord("apple", 32, "six");
    goods.writeRecord("beef", 12, "Tom");
    goods.writeRecord("car", 64, "Tom");
    goods.writeRecord("robet", 93, "Mike");
    goods.writeRecord("bowl", 5, "Smith");
    goods.writeRecord("shirt", 247, "Green");
    goods.writeRecord("robet", 93, "Green");
    goods.writeRecord("oil", 357, "five");
    goods.writeRecord("peanut", 256, "Smith");

    screen.printScreen();

    while (screen.getCommand(goods));

    screen.leave();
}
#endif // WRITEPAGE1
