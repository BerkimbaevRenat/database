#include "Schedule.h"


void Schedule::exportToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("output error");
    }
    std::vector<std::vector<std::string>> res;
    for(const auto &teacher: this->teacherHash) {
        std::vector<std::string> buf;
        for(const auto &point: teacher.second) {
            buf.push_back(std::to_string(point.first));
            buf.push_back(std::to_string(point.second));
            std::vector<std::string> vectorCell = convertCellToVector(this->getCell(point.first,point.second));
            buf.insert(buf.end(),vectorCell.begin(),vectorCell.end());
            res.push_back(buf);
        }
    }
    std::vector<std::string> stringData = joinVector(res);

    for(const auto &oneLine: stringData) {
        file << oneLine;
    }
    file.close();
}

void Schedule::importFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> buf;
    std::vector<std::vector<std::string>> res;
    std::string read;
    while (getline(file, read))
    {
        buf = fromStringToVectorString(read);
        buf.erase(buf.begin());
        buf.erase(buf.begin());
        res.push_back(buf);
    }

    std::vector<CellMatrix> vectorToCell = this->convertVectorToCell(res);

    for(const auto &cell: vectorToCell) {
        this->addCell(cell);
    }
}