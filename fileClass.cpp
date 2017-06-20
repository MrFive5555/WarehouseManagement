#include <ctime>
#include "fileClass.h"
#include "complieOption.h"

fileClass::fileClass()
{
    dataFile.open("data/Goods.data", fstream::out | fstream::ate | fstream::in);
    if (!dataFile) {
        dataFile.open("data/Goods.data", fstream::in | fstream::out | fstream::ate |  fstream::trunc);
    }
    while (dataFile.tellp() < DATA_LEGNTH) {
        dataFile << " ";
    }

    pointFile.open("data/filePointer.data", fstream::out | fstream::ate | fstream::in);
    if (!pointFile) {
        pointFile.open("data/filePointer.data", fstream::in | fstream::out | fstream::ate | fstream::trunc);
    }
    while (pointFile.tellp() < POINTER_LENGTH) {
        pointFile << " ";
    }

    hisFile.open("data/History.data", fstream::out | fstream::ate | fstream::in);
    if (!hisFile) {
        hisFile.open("data/History.data", fstream::in | fstream::out | fstream::ate | fstream::trunc);
    }
    while (hisFile.tellp() < HISTORY_LENGTH) {
        // << hisFile.tellp() << endl;
        hisFile << " ";
    }
}

fileClass::~fileClass()
{
    dataFile.clear();
    dataFile.close();

    pointFile.clear();
    pointFile.close();

    hisFile.clear();
    hisFile.close();
}

fileClass::STATE fileClass::writeRecord(string goodsName, int count, string AdminiName)
{
    if (!normal()) {
        fresh(); return OPEN_FILE_ERROR;
    }

    auto pos = getRecordPosition(goodsName);

    //get time time
    char nowTime[20];
    time_t t = time(0);
    strftime(nowTime, sizeof(nowTime), "%F-%R", localtime(&t));


    if (pos > NO_RECORD) { //if there is old record

        //get old record
        dataFile.seekg(pos, 0);
        int nowCount = 0;
        string temp;
        int hisPos = 0; //I can't use fstream::pos_type hisPos; dataFile >> hisPos; so use int instead of fstream::pos_type
        dataFile >> temp >> nowCount >> hisPos;

        //write new record
        if ((nowCount + count) >= 0) {
            nowCount += count;
        } else {
            fresh(); return NO_ENOUGH_GOODS;
        }
        dataFile.seekp(pos, 0);

        //write history
        writeHistory(goodsName, nowCount, count, AdminiName, nowTime, static_cast<fstream::pos_type>(hisPos));

        //should not change pointer

        //write in file
        dataFile << goodsName << " " << nowCount << " " << hisPos << " " << nowTime << " " << AdminiName << " ";
        while (dataFile.tellp() < pos + DATA_LEGNTH) {
            dataFile << " ";
        }

    } else { //if there is no old record
        if (count < 0) {
            fresh(); return NO_THIS_GOODS;
        }

        dataFile.seekp(0, fstream::end);

        //write history
        fstream::pos_type hisPos = 0;
        hisPos = writeHistory(goodsName, count, count, AdminiName, nowTime, hisPos);

        //write new pointer
        pos = dataFile.tellp();
        writePointer(goodsName, pos);

        //write new record
        dataFile << goodsName << " " << count << " " << hisPos << " " << nowTime << " " << AdminiName << " ";
        while (dataFile.tellp() < (pos + DATA_LEGNTH)) {
            dataFile << " ";
        }
    }
    fresh(); return SUCCESS;
}

bool fileClass::getRecord(fstream::pos_type recordPos, string& goodsName,
    int& retCount, fstream::pos_type& retHisPos, string& retTime,
    string& retAdminName)
{
    if (!normal()) {
        fresh(); return false;
    }

    dataFile.seekg(recordPos);
    int hisPos = 0; //I can't use fstream::pos_type hisPos; dataFile >> hisPos; so use int instead of fstream::pos_type
    dataFile >> goodsName >> retCount >> hisPos >> retTime >> retAdminName;
    retHisPos = static_cast<fstream::pos_type> (hisPos);
    fresh(); return true;
}

//return NO_RECORD if there is no record in file "history.data"
fstream::pos_type fileClass::getRecordPosition(string goodsName)
{
    pointFile.seekg(0, fstream::end);
    if (pointFile.tellg() == POINTER_LENGTH) { //no record
        fresh(); return NO_RECORD;
    }

    //binary search
    fstream::pos_type end = pointFile.tellg() / POINTER_LENGTH;
    pointFile.seekg(POINTER_LENGTH);
    fstream::pos_type beg = pointFile.tellg() / POINTER_LENGTH;
    //cout << "end : " << end << "beg : " << endl;
    string temp;
    pointFile >> temp;
    int recordPos = 0; //I can't use fstream::pos_type hisPos; dataFile >> hisPos; so use int instead of fstream::pos_type

    //binary search
    while (beg < end - fstream::pos_type(1)) {
        fstream::pos_type mid = (beg + end) / 2;
        pointFile.seekg(mid * POINTER_LENGTH);
        pointFile >> temp;

        if (temp == goodsName) {
            pointFile >> recordPos;
            fresh(); return static_cast<fstream::pos_type>(recordPos);
        } else if (temp > goodsName) {
            end = mid;
        } else {
            beg = mid;
        }
        pointFile.seekg(beg * POINTER_LENGTH);
        pointFile >> temp;
    }

    if (temp == goodsName) {
        pointFile >> recordPos;
        fresh(); return static_cast<fstream::pos_type>(recordPos);
    } else {
        pointFile.seekg(end * POINTER_LENGTH);
        pointFile >> temp;
        if (temp == goodsName) {
            pointFile >> recordPos;
            fresh(); return static_cast<fstream::pos_type>(recordPos);
        }
    }
    fresh(); return 0;
}

bool fileClass::writePointer(string goodsName, fstream::pos_type recordPointer)
{
    if (!normal()) {
        fresh(); return false;
    }
    pointFile.seekp(0, fstream::end);
    //find the correct position of the new pointer
    //the pointer in file "filePointer.data" should be sorted in ascending
    //we use insertion sort

    // 1.we find the correct position
    // (binary search)
    fstream::pos_type end = pointFile.tellg() / POINTER_LENGTH;
    pointFile.seekg(POINTER_LENGTH);
    fstream::pos_type beg = pointFile.tellg() / POINTER_LENGTH;
    string temp;    //temp will not be equal to goodsName, used to get goods name in file "filePointer.data"
    fstream::pos_type insertPos = 0;

    if (end == beg) { //there is no old pointer in file "filePointer.data"
        insertPos = POINTER_LENGTH; //insert at the first position
    } else { //there is old pointer in file "filePointer.data"
        pointFile.seekg(beg * POINTER_LENGTH);
        pointFile >> temp; //initialize the string temp, otherwise if beg = end - 1, the temp will be empty string

        while (beg < end - fstream::pos_type(1)) {
            fstream::pos_type mid = (beg + end) / 2;
            pointFile.seekg(mid * POINTER_LENGTH);
            pointFile >> temp;

            //cout << "beg ; " << beg << endl << "end : " << end << endl << endl;

            if (temp > goodsName) {
                end = mid;
            } else {
                beg = mid;
            }
            pointFile.seekg(beg * POINTER_LENGTH);
            pointFile >> temp;
        }

        insertPos = (goodsName > temp) ? end * POINTER_LENGTH : beg * POINTER_LENGTH;
    }

    // 2.move the record after the new record
    fstream::pos_type move = fileEnd(pointFile); 

    while (move != insertPos) {

        pointFile.seekg(move - POINTER_LENGTH);

        string name;
        int position;

        pointFile >> name >> position;  //bad()

        pointFile.seekp(move);

        pointFile << name << " " << position;
        while (pointFile.tellp() != move + POINTER_LENGTH) {//infinited run
            pointFile << " ";
        }

        move = move - POINTER_LENGTH;
    }

    // 3.write the new record
    pointFile.seekp(insertPos);

    pointFile << goodsName << " " << recordPointer;
    while (pointFile.tellp() < insertPos + POINTER_LENGTH) {
        pointFile << " ";
    }

    fresh(); return true;
}

fstream::pos_type fileClass::fileEnd(fstream& fstr)
{
    fstr.seekg(0, fstream::end);
    fresh(); return fstr.tellg();
}

void fileClass::fresh()
{
    dataFile << flush;
    pointFile << flush;

    dataFile.clear();
    pointFile.clear();
}

bool fileClass::normal()
{
    if (!dataFile || !pointFile || !hisFile) {
        return false;
    } else {
        return true;
    }
}

fstream::pos_type fileClass::writeHistory(string goodsName, int nowCount, int count, string AdminiName, char* nowTime, fstream::pos_type& hisPos)
{
    fstream::pos_type next = hisPos;
    fstream::pos_type now = 0;
    History his;

    hisFile.seekg(0, fstream::end);

    while (next != static_cast<fstream::pos_type>(0)) {
        hisFile.seekg(next);
        now = next;
        hisFile >> his;
        next = his.nextPos;
    }
    if (!his.goodsName.empty()) { //if there are old history
        hisFile.seekg(0, fstream::end);
        his.nextPos = hisFile.tellg(); //the end

        hisFile.seekp(now);
        hisFile << his;
    }
    
    hisFile.seekp(0, fstream::end);
    now = hisFile.tellp();
    hisFile << goodsName << " " << count << " " << nowCount << " " << AdminiName << " " << nowTime << " " << 0;
    while (hisFile.tellp() != now + HISTORY_LENGTH) {
        hisFile << " ";
    }

    fresh(); return now;
}

fstream::pos_type fileClass::getHistoryPosition(string goodsName)
{
    fstream::pos_type pos = getRecordPosition(goodsName);
    if (pos == NO_RECORD) {
        return NO_RECORD;
    }
    
    dataFile.seekg(pos);

    int hisPos = 0; //I can't use fstream::pos_type hisPos; dataFile >> hisPos; so use int instead of fstream::pos_type
    int count = 0;
    string time;
    string AdminName;
    dataFile >> goodsName >> count >> hisPos >> time >> AdminName;
    return static_cast<fstream::pos_type> (hisPos);
}

History fileClass::getHistory(fstream::pos_type hisPos)
{
    History his;
    hisFile.seekg(hisPos);
    hisFile >> his;
    return his;
}

fstream& operator >> (fstream& fstr, History& his)
{
    int pos = 0;
    fstr >> his.goodsName >> his.changeNumber >> his.stock >> his.adminName >> his.time >> pos;
    his.nextPos = pos;
    return fstr;
}

fstream& operator << (fstream& fstr, const History& his)
{
    fstr << his.goodsName << " " <<  his.changeNumber << " " << his.stock << " " << his.adminName << " " << his.time << " " << his.nextPos;
    return fstr;
}