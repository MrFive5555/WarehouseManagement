#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//file structure
//  Goods.data : the file storing the stock record
//      (the first position in "Goods.data" are not used for store but used as a mark)
//  filePoint.data : the file storing the point which can help locating record in "Goods.data"
//      (the name in filePointer.data will be sorted to enhance the effeciency of search)
//  History.data : the file storing the import and export history
//      (the first in "History.data" are not used for store but used as a mark)

//store format

#define DATA_LEGNTH static_cast<fstream::pos_type>(65)
//a goods name with 17 characters
//a count with 6 characters
//a history pointer with 5 characters
//a string of last changed time with 16 characters
//administrator name with 15 characters
//  used in file "Goods.data"
//      e.g :
//      milk 25 10 2017-01-05-16:33 Tom
//      mean : there are 25 milk in store, used "hisFile.seekg(10 * HISTORY_LENGTH, 0)" can locate the history in "History.data"
//             and the last operation is performed by Tom at 16:33 at Jan.5th in 2017

#define POINTER_LENGTH static_cast<fstream::pos_type>(25)
//a goods name with 17 characters
//a pointer with 5 characters
//  used in file "filePointer.data"
//      e.g :
//      milk 30
//      mean : used "dataFile.seekg(30 * DATA_LENGTH, 0)" can locate the record in "Goods.data"

#define HISTORY_LENGTH static_cast<fstream::pos_type>(70)
//a goods name with 17 characters
//change number with 6 characters
//the stock number now with 6 character
//administrator name with 15 characters
//operating time with 16 characters
//a history pointer with 5 characters
//  used in file "History.data"
//      e.g 1 :
//      milk +13 34 Tom 2017-03-14 70
//      mean : the last operate for milk is in 2017-03-14, operated by Tom, he add 13 milk, there are 34 milk in warehouse now
//             used "hisFile.seekg(0, HISTORY_LENGTH)" can locate the history in "History.data"
//      e.g 2 :
//      orange -2 25 Mike 2017-04-15 0
//      mean : the last operate for milk is in 2017-04-15, operated by Mike, he take 2 orange, there are 25 orange in warehouse now
//             it is the last history of orange

using namespace std;

#ifndef FILECLASS_H_
#define FILECLASS_H_
#define NO_RECORD static_cast<fstream::pos_type>(0)
//if there is no record, return NO_RECORD

class History{
public:
    string goodsName;
    int changeNumber;
    int stock;
    string adminName;
    string time;
    fstream::pos_type nextPos;
};

fstream& operator >> (fstream& fstr, History& his);
fstream& operator << (fstream& fstr, const History& his);

class fileClass
{
public:
    typedef enum {
        NO_ENOUGH_GOODS,
        NO_THIS_GOODS,
        OPEN_FILE_ERROR,
        SUCCESS
    } STATE;

    fileClass();
    ~fileClass();
    STATE writeRecord(string goodsName, int count, string AdminiName);
    bool getRecord(fstream::pos_type recordPos, string& goodsName,
        int& retCount, fstream::pos_type& retHisPos, string& retTime,
        string& retAdminName);
    fstream::pos_type getRecordPosition(string goodsName);
    bool writePointer(string goodsName, fstream::pos_type recordPointer);

    fstream::pos_type writeHistory(string goodsName, int nowCount, int count, string AdminiName, char* nowTime, fstream::pos_type& hisPos);
    fstream::pos_type getHistoryPosition(string goodsName);
    History getHistory(fstream::pos_type hisPos);

    fstream dataFile;
    fstream pointFile;
    fstream hisFile;
    void fresh();
    fstream::pos_type fileEnd(fstream& fstr);

private:
    bool normal();
};

#endif // !FILECLASS_H_