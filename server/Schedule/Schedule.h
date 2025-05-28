#ifndef SERVER_SCHEDULE_H
#define SERVER_SCHEDULE_H
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "../tools/tools.hpp"
//#include <hash_set>

//insert teacher=Сидоров group=402 subject=Геометрия date=10.10.2024 numberlesson=2 classroom=708 end
//select teacher=Иванов group=101 subject=Алгебра date=10.10.2024 numberlesson=1 classroom=101 end
struct CellMatrix {
    std::string date;
    size_t numberLesson;
    size_t classroom;
    std::string subject;
    std::string teacher;
    size_t group;
    bool is_null = true;
};

class Schedule {
public:
    //Init
    Schedule();
    ~Schedule() {this->exportToFile("data.dat");};



    //add cell
    bool addCell(std::string date,size_t numberLesson,size_t classroom,std::string subject,std::string teacher,size_t group);
    bool addCell(CellMatrix cell);
    bool forceAddCell(std::string date,size_t numberLesson,size_t classroom,std::string subject,std::string teacher,size_t group);
    bool forceAddCell(CellMatrix cell);
    bool setCell(CellMatrix cell);

    //remove cell
    void removeCell(CellMatrix cell);
    void removeCell(size_t group,std::string date, size_t numberLesson);

    //проверка накладки
    bool checkUnion(CellMatrix cell);


    //convert vector pair to vector cell
    std::vector<CellMatrix> convertPairToCell(std::vector<std::pair<int,int>> pairs);
    std::vector<std::vector<std::string>> convertCellToVector(std::vector<CellMatrix> Cells);
    std::vector<std::string> convertCellToVector(CellMatrix Cell);
    std::vector<CellMatrix> convertVectorToCell(std::vector<std::vector<std::string>> Cells);


    //select in Select.cpp
    std::vector<std::pair<int,int>> getCellByTeacher(std::string teacher);
    std::vector<std::pair<int,int>> getCellBySubject(std::string subject);
    std::vector<std::pair<int,int>> getCellByGroup(std::vector<int> group);
    std::vector<std::pair<int,int>> getCellByClassroom(std::vector<int> classrooms);
    std::vector<std::pair<int,int>> getCellByDate(std::string date);
    std::vector<std::pair<int,int>> subPair(std::vector<std::vector<std::pair<int,int>>> pair);


private:
    //get-set cell

    CellMatrix getCell(size_t col,size_t row);

    //convert to row
    int convertToRow(CellMatrix cell);
    int convertToRow(std::string date, size_t numberLesson);

    //parse tools
    std::vector<int> parseStringToVectorDay(std::string date);
    size_t numberDayInYear(std::vector<int> dateInVector);

    //import-export from file
    void exportToFile(const std::string& filename);
    void importFromFile(const std::string& filename);

    //hash_map
    bool removeFromSubject(const std::string& subject,int col,int row);
    bool addToSubject(const std::string& subject,int col,int row);

    bool removeFromTeacher(const std::string& teacher,int col,int row);
    bool addToTeacher(const std::string& teacher,int col,int row);


    //get hash
    std::vector<std::pair<int,int>> getTeacherValue(const std::string& teacher);
    std::vector<std::pair<int,int>> getSubjectValue(const std::string& subject);


    //local variables
    size_t countDayInYear = 365;
    size_t maxLessonInDay = 8;
    size_t countGroupInCourse = 100;
    size_t countCourse = 5;

    std::vector<int> classrooms;
    std::vector<int> dayInMonth = {31,29,31,30,31,30,31,31,30,31,30,31};

    std::vector<std::vector<CellMatrix>> matrix;
    CellMatrix nullCell;
    std::unordered_map<std::string,std::vector<std::pair<int,int>>> teacherHash;
    std::unordered_map<std::string,std::vector<std::pair<int,int>>> subjectHash;
};



#endif //SERVER_SCHEDULE_H
