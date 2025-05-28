//
// Created by mars on 20.05.2024.
//
#include <utility>

#include "Schedule.h"

std::vector<std::pair<int,int>> Schedule::getCellByTeacher(std::string teachers) {
    return this->getTeacherValue(std::move(teachers));
}
std::vector<std::pair<int,int>> Schedule::getCellBySubject(std::string subjects){
    return this->getSubjectValue(std::move(subjects));
}
std::vector<std::pair<int,int>> Schedule::getCellByGroup(std::vector<int> groups){
    std::vector<std::pair<int,int>> res;
    bool is_null;
    for(const auto &group: groups) {
        for(int i =0;i<this->matrix[group].size();++i) {
            is_null = this->matrix[group][i].is_null;
            if(!is_null) {
                res.push_back(std::pair<int,int>(group,i));
            }
        }
    }
    return res;
}

std::vector<std::pair<int,int>> Schedule::getCellByClassroom(std::vector<int> classrooms) {
    std::vector<std::pair<int,int>> res;
    for(int i =0;i<this->matrix.size();++i) {
        for(int j =0;j<this->matrix[i].size();++j) {
            if(!this->matrix[i][j].is_null) {
                if (std::find(classrooms.begin(),classrooms.end(),this->matrix[i][j].classroom) != classrooms.end()) {
                    res.push_back(std::pair<int,int>(i,j));
                }
            }
        }
    }
    return res;
}

std::vector<std::pair<int,int>> Schedule::getCellByDate(std::string date) {
    std::vector<std::pair<int,int>> res;
    for(int i =0;i< matrix.size();++i) {
        for(int j =0;j< 8;++j) {
            int row = this->convertToRow(date,j);
            if(! this->matrix[i][row].is_null) {
                res.push_back(std::pair<int,int>(i,row));
            }
        }
    }
    return res;
}

std::vector<std::pair<int,int>> Schedule::subPair(std::vector<std::vector<std::pair<int,int>>> pairs) {
    std::vector<std::pair<int,int>> res;
    if (pairs.size() == 0) {
        return {};
    }
    else if (pairs.size() == 1) {
        return pairs[0];
    }
    else {
        for (const auto &pair: pairs[0]) {
            bool findInOther = true;
            for (const auto &subVector: pairs) {
                if (std::find(subVector.begin(),subVector.end(),pair) == subVector.end())
                    findInOther = false;
            }
            if (findInOther) {
                res.push_back(pair);
            }
        }
    }
    return res;
}