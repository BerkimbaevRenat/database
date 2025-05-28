
#include <utility>

#include "Schedule.h"

/*
 *
 * Init
 *
 */
Schedule::Schedule() {
    //basic init
    size_t countCol = this->countCourse * this->countGroupInCourse;
    size_t countRow = 365 * this->maxLessonInDay;
    this->classrooms.reserve(1000);
    for(int i =0;i<1000;++i) {
        classrooms.push_back(i);
    }
    CellMatrix null;
    null.is_null = true;
    this->nullCell = null;

    std::vector<std::vector<CellMatrix>> buf;
    buf.reserve(countCol);
    std::vector<CellMatrix> subBuf;
    subBuf.reserve(countRow);
    for(int i =0;i<countRow;++i) {
        subBuf.push_back(this->nullCell);
    }
    for (int i =0;i< countCol; ++i) {
        buf.push_back(subBuf);
    }

    this->matrix = buf;

    //restore file
    std::ifstream  fileData("data.dat");

    if (fileData.bad()) {
        std::cout << "File backup doesn't find";
    }
    else {
        this->importFromFile("data.dat");
    }

}

/*
 *
 * add cell
 *
 */
bool Schedule::addCell(std::string date,size_t numberLesson,size_t classroom,std::string subject,std::string teacher,size_t group)
{
    //initialize struct
    CellMatrix buffer;
    buffer.is_null = false;
    buffer.date = std::move(date);
    buffer.numberLesson = numberLesson;
    buffer.classroom = classroom;
    buffer.subject = std::move(subject);
    buffer.teacher = std::move(teacher);
    buffer.group = group;

    //add cell to matrix
    CellMatrix inThisTime = this->getCell(buffer.group,convertToRow(buffer));
    if (inThisTime.is_null) {
        if(this->setCell(buffer)) {
            this->addToSubject(buffer.subject,buffer.group,convertToRow(buffer));
            this->addToTeacher(buffer.teacher,buffer.group,convertToRow(buffer));
            return true;
        }
        return false;
    }

    return false;
}


bool Schedule::addCell(CellMatrix cell)
{

    //add cell to matrix
    CellMatrix inThisTime = this->getCell(cell.group,convertToRow(cell));
    if (inThisTime.is_null) {
        if(this->setCell(cell)) {
            this->addToSubject(cell.subject,cell.group,convertToRow(cell));
            this->addToTeacher(cell.teacher,cell.group,convertToRow(cell));
            return true;
        }
        return false;
    }

    return false;
}


bool Schedule::forceAddCell(std::string date,size_t numberLesson,size_t classroom,std::string subject,std::string teacher,size_t group)
{
    //initialize struct
    CellMatrix buffer;
    buffer.is_null = false;
    buffer.date = date;
    buffer.numberLesson = numberLesson;
    buffer.classroom = classroom;
    buffer.subject = subject;
    buffer.teacher = teacher;
    buffer.group = group;

    //add cell to matrix
    CellMatrix inThisTime = this->getCell(buffer.group,convertToRow(buffer));

    //удалить старую
    this->removeFromSubject(buffer.subject,buffer.group,convertToRow(buffer));
    this->removeFromTeacher(buffer.teacher,buffer.group,convertToRow(buffer));

    this->setCell(buffer);

    this->addToSubject(buffer.subject,buffer.group,convertToRow(buffer));
    this->addToTeacher(buffer.teacher,buffer.group,convertToRow(buffer));

    return true;
}
bool Schedule::forceAddCell(CellMatrix cell)
{

    //add cell to matrix
    CellMatrix inThisTime = this->getCell(cell.group,convertToRow(cell));

    //удалить старую
    this->removeFromSubject(cell.subject,cell.group,convertToRow(cell));
    this->removeFromTeacher(cell.teacher,cell.group,convertToRow(cell));

    this->setCell(cell);

    this->addToSubject(cell.subject,cell.group,convertToRow(cell));
    this->addToTeacher(cell.teacher,cell.group,convertToRow(cell));

    return true;
}


/*
 *
 * remove cell
 *
 */
void Schedule::removeCell(size_t group,std::string date,size_t numberLessons) {
    CellMatrix buf = this->getCell(group,convertToRow(date,numberLessons));
    this->removeFromSubject(buf.subject,buf.group,convertToRow(buf));
    this->removeFromTeacher(buf.teacher,buf.group,convertToRow(buf));
    this->matrix[group][convertToRow(date,numberLessons)] = this->nullCell;
}
void Schedule::removeCell(CellMatrix cell) {
    CellMatrix buf = this->getCell(cell.group,convertToRow(cell.date,cell.numberLesson));
    this->removeFromSubject(buf.subject,buf.group,convertToRow(buf));
    this->removeFromTeacher(buf.teacher,buf.group,convertToRow(buf));
    this->matrix[cell.group][convertToRow(cell.date,cell.numberLesson)] = this->nullCell;
}


/*
 *
 * convert vector pair to vector cell
 *
 */
std::vector<CellMatrix> Schedule::convertPairToCell(std::vector<std::pair<int,int>> pairs){
    std::vector<CellMatrix> res;
    res.reserve(pairs.size());
    if (pairs.size() == 0) {
        return {};
    }
    for (const auto &pair: pairs) {
        res.push_back(this->getCell(pair.first,pair.second));
    }
    return res;
}
std::vector<std::vector<std::string>> Schedule::convertCellToVector(std::vector<CellMatrix> Cells){
    std::vector<std::vector<std::string>> res;
    for(const auto &cell: Cells) {
        std::vector<std::string> buf;
        buf.push_back(cell.teacher);
        buf.push_back(cell.subject);
        buf.push_back(cell.date);
        buf.push_back(std::to_string(cell.group));
        buf.push_back(std::to_string(cell.numberLesson));
        buf.push_back(std::to_string(cell.classroom));
        res.push_back(buf);
    }
    return res;
}
std::vector<std::string> Schedule::convertCellToVector(CellMatrix cell){
    std::vector<std::string> buf;
    buf.push_back(cell.teacher);
    buf.push_back(cell.subject);
    buf.push_back(cell.date);
    buf.push_back(std::to_string(cell.group));
    buf.push_back(std::to_string(cell.numberLesson));
    buf.push_back(std::to_string(cell.classroom));
    return buf;
}
std::vector<CellMatrix> Schedule::convertVectorToCell(std::vector<std::vector<std::string>> Cells){
    std::vector<CellMatrix> res;
    for(const auto &cell: Cells) {
        CellMatrix buf;
        buf.teacher = cell[0];
        buf.subject = cell[1];
        buf.date = cell[2];
        buf.group = std::stoi(cell[3]);
        buf.numberLesson = std::stoi(cell[4]);
        buf.classroom = std::stoi(cell[5]);
        buf.is_null = false;
        res.push_back(buf);
    }
    return res;
}

/*
 *
 *
 * проверка наклдаки
 * true  - если нет накладок
 * false - если есть
 *
 */
bool Schedule::checkUnion(CellMatrix cell){
    int row = this->convertToRow(cell);
    for(int i =0;i<this->matrix.size();++i) {
        if(cell.classroom == this->matrix[i][row].classroom) {
            return false;
        }
    }
    return true;
}


/*
 *
 * get-set cell
 *
 */
CellMatrix Schedule::getCell(size_t col, size_t row) {
    if (this->matrix.size() <= col | this->matrix[0].size() <= row ) {
        return this->nullCell;
    }
    return this->matrix[col][row];

}

bool Schedule::setCell(CellMatrix cell) {
    size_t col = cell.group;
    int row = this->numberDayInYear(this->parseStringToVectorDay(cell.date)) + cell.numberLesson;
    CellMatrix cellInMatrix = this->getCell(col,row+cell.numberLesson-1);
    bool is_null = cellInMatrix.is_null;
    if (is_null) {
        this->matrix[col][row] = cell;
        return true;
    }
    return false;
}

/*
 *
 * convert to row
 *
 */
int Schedule::convertToRow(CellMatrix cell) {
    return this->numberDayInYear(this->parseStringToVectorDay(cell.date)) + cell.numberLesson;
}
int Schedule::convertToRow(std::string date,size_t numberLesson) {
    return this->numberDayInYear(this->parseStringToVectorDay(date)) + numberLesson;
}

/*
 *
 * parse tools
 *
 */
size_t Schedule::numberDayInYear(std::vector<int> dateInVector) {
    if (dateInVector.size() != 3) {
        return 0;
    }
    int countDay = 0;
    for(int i =0;i<dateInVector[1]-1;++i) {
        countDay += this->dayInMonth[i];
    }
    // 10 день * 8 = начиная с 80 строки будет 8 строк о 10 дне
    return (countDay+dateInVector[0]-1)*this->maxLessonInDay;
}

std::vector<int> Schedule::parseStringToVectorDay(std::string date) {
    std::vector<std::string> splitToVectorString = split(std::move(date),'.');
    std::vector<int> splitToVectorInt;
    splitToVectorInt.reserve(splitToVectorString.size());
    for (auto &c: splitToVectorString) {
        try {
            splitToVectorInt.push_back(std::stoi(c));
        }
        catch (...) {
            return {};
        }
    }
    return splitToVectorInt;
}


/*
 *
 * hash map
 *
 */
bool Schedule::addToSubject(const std::string& subject, int col, int row) {
    std::vector<std::pair<int,int>> buf;
    if (this->subjectHash.contains(subject)) {
        buf = this->subjectHash[subject];
    }
    buf.push_back(std::pair<int,int>(col,row));
    this->subjectHash[subject] = buf;
    return true;
}

bool Schedule::removeFromSubject(const std::string& subject, int col, int row) {
    std::vector<std::pair<int,int>> buf;
    if (this->subjectHash.contains(subject)) {
        buf = this->subjectHash.at(subject);
        buf.erase(std::find(buf.begin(),buf.end(), std::pair<int,int>(col,row)));
        this->subjectHash[subject] = buf;
        return true;
    }
    return false;
}

bool Schedule::removeFromTeacher(const std::string& teacher, int col, int row) {
    std::vector<std::pair<int,int>> buf;
    if (this->teacherHash.contains(teacher)) {
        buf = this->teacherHash[teacher];
    }
    buf.push_back(std::pair<int,int>(col,row));
    this->teacherHash[teacher] = buf;
    return true;
}

bool Schedule::addToTeacher(const std::string& teacher, int col, int row) {
    std::vector<std::pair<int,int>> buf;
    if (this->teacherHash.contains(teacher)) {
        buf = this->teacherHash[teacher];
    }
    buf.push_back(std::pair<int,int>(col,row));
    this->teacherHash[teacher] = buf;
    return true;
}


/*
 *
 * get hash
 *
 */
std::vector<std::pair<int, int>> Schedule::getTeacherValue(const std::string& teacher) {
    if (this->teacherHash.contains(teacher)) {
        return this->teacherHash[teacher];
    }
    return {};
}

std::vector<std::pair<int, int>> Schedule::getSubjectValue(const std::string& subject) {
    if (this->subjectHash.contains(subject)) {
        return this->subjectHash[subject];
    }
    return {};
}

