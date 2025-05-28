//
// Created by mars on 21.05.2024.
//
#include "AnalyseQuery.h"



std::string AnalyseQuery::analyse(const std::string &query) {
    if(!checkQuery(query)) {
        return "Error query! Bad sintaksis!";
    }

    std::string command = this->getCommand(query);
    if (command == "select") {
        return this->analyseSelect(query);
    }
    else if (command == "remove") {
        return this->analyseRemove(query);
    }
    else if (command == "insert") {
        return this->analyseInsert(query);
    }
    else {
        return "Error query! There is no command!";
    }

}

std::string AnalyseQuery::analyseSelect(const std::string &query) {
    std::map<std::string,std::string> getField = this->getField(query);
    std::vector<std::vector<std::pair<int,int>>> composePair;
    for (const auto &param: getField) {
        if (param.first == "date") {
            composePair.push_back(this->schedule->getCellByDate(param.second));
        }
        else if (param.first == "numberlesson") {//TODO::Разобраться куда пары включить
            //buffer.push_back({});
            int xz =1;
        }
        else if (param.first == "classroom") {
            std::vector<int> vectorClassroms;
            if (param.second.contains('-')) {
                std::vector<std::string> beginEndClassroms = split(param.second,'-');
                for(int i = std::stoi(beginEndClassroms[0]);i<=std::stoi(beginEndClassroms[1]);++i) {
                    vectorClassroms.push_back(i);
                }
            }
            else {
                vectorClassroms.push_back(std::stoi(param.second));
            }
            composePair.push_back(this->schedule->getCellByClassroom(vectorClassroms));
        }
        else if (param.first == "subject") {
            composePair.push_back(this->schedule->getCellBySubject(param.second));
        }
        else if (param.first == "teacher") {
            composePair.push_back(this->schedule->getCellByTeacher(param.second));
        }
        else if (param.first == "group") {
            std::vector<int> vectorGroup;
            if (param.second.contains('-')) {
                std::vector<std::string> beginEndGroup = split(param.second,'-');
                for(int i = std::stoi(beginEndGroup[0]);i<=std::stoi(beginEndGroup[1]);++i) {
                    vectorGroup.push_back(i);
                }
            }
            else {
                vectorGroup.push_back(std::stoi(param.second));
            }
            composePair.push_back(this->schedule->getCellByGroup(vectorGroup));
        }
        else {
            return "Error query! There is no field!";
        }
    }
    std::vector<std::pair<int,int>> submit = this->schedule->subPair(composePair);
    std::vector<CellMatrix> resCell = this->schedule->convertPairToCell(submit);
    std::vector<std::vector<std::string>> resTable = this->schedule->convertCellToVector(resCell);
    std::string serializeData = SerializeVectorsToSend(this->headerToSend,joinVector(resTable));

    return serializeData;
}

std::string AnalyseQuery::analyseInsert(const std::string &query) {
    std::map<std::string,std::string> getField = this->getField(query);
    std::vector<std::vector<std::pair<int,int>>> composePair;
    if (getField.size() != 6) {
        return "Error query! Not full field in struct";
    }
    CellMatrix buf;
    for (const auto &param: getField) {
        if (param.first == "date") {
            buf.date = param.second;
        }
        else if (param.first == "numberlesson") {
            buf.numberLesson = std::stoi(param.second);
        }
        else if (param.first == "classroom") {
            buf.classroom = std::stoi(param.second);
        }
        else if (param.first == "subject") {
            buf.subject = param.second;
        }
        else if (param.first == "teacher") {
            buf.teacher = param.second;
        }
        else if (param.first == "group") {
            buf.group = std::stoi(param.second);
        }
        else {
            return "Error query! There is no field! \"" + param.second+" \"";
        }
    }
    buf.is_null = false;
    if (!this->schedule->checkUnion(buf)) {
        return "Error! This classroom in busy!";
    }
    bool complete = this->schedule->addCell(buf);
    if (complete) {
        return "Complete add cell to schedule";
    }
    return "Error query! Some error";
}

std::string AnalyseQuery::analyseRemove(const std::string &query) {
    std::map<std::string,std::string> getField = this->getField(query);
    std::vector<std::vector<std::pair<int,int>>> composePair;
    if (getField.size() != 3) {
        return "Error query! Not full field in struct";
    }
    CellMatrix buf;
    for (const auto &param: getField) {
        if (param.first == "date") {
            buf.date = param.second;
        }
        else if (param.first == "numberlesson") {
            buf.numberLesson = std::stoi(param.second);
        }
        else if (param.first == "group") {
            buf.group = std::stoi(param.second);
        }
        else {
            return "Error query! There is no field! \"" + param.second+" \"";
        }
    }
    buf.is_null = false;
    this->schedule->removeCell(buf);
    return "Removed is ok";
}



//get get
std::string AnalyseQuery::getCommand(const std::string &query) {
    std::vector<std::string> splitQuery = split(query,' ');
    auto it = std::find(this->allowCommand.begin(),this->allowCommand.end(),splitQuery[0]);
    if (it != this->allowCommand.end()){
        return splitQuery[0];
    }
    return {};
}

std::map<std::string,std::string> AnalyseQuery::getField(const std::string &query) {
    std::vector<std::string> splitQuery = split(query,' ');
    std::vector<std::string>::iterator iter;
    if (std::find(splitQuery.begin(),splitQuery.end(),"sort") != splitQuery.end()) {
        iter = std::find(splitQuery.begin(),splitQuery.end(),"sort");
    }
    else {
        iter = std::find(splitQuery.begin(),splitQuery.end(),"end");
    }
    std::vector<std::string> fields = std::vector<std::string>(splitQuery.begin()+1,iter);
    std::map<std::string,std::string> res;
    for(int i =0;i<fields.size();++i) {
        std::string param  = split(fields[i],'=')[0];
        std::string value  = split(fields[i],'=')[1];
        res[param] = value;
    }
    return res;
}













//соединение всех проверок
bool AnalyseQuery::checkQuery(const std::string& query) {
    if(this->checkCorrectCommand(query) && this->checkSort(query) && this->checkCorrectField(query) && this->checkCorrectFinish(query)) {
        return true;
    }
    return false;
}

//проверка запроса команды
bool AnalyseQuery::checkCorrectCommand(const std::string& query) {
    std::vector<std::string> splitQuery = split(query,' ');
    if (std::find(this->allowCommand.begin(),this->allowCommand.end(),splitQuery[0]) != this->allowCommand.end()){
        return true;
    }
    return false;
}

//проверка конца запроса
bool AnalyseQuery::checkCorrectFinish(const std::string& query) {
    std::vector<std::string> splitQuery = split(query,' ');
    if (splitQuery[splitQuery.size()-1] == "end"){
        return true;
    }
    return false;
}

//проверка полей
bool AnalyseQuery::checkCorrectField(const std::string &query) {
    std::vector<std::string> splitQuery = split(query,' ');
    std::vector<std::string>::iterator iter;
    if (std::find(splitQuery.begin(),splitQuery.end(),"sort") != splitQuery.end())
    {
        iter = std::find(splitQuery.begin(),splitQuery.end(),"sort");
    }
    else {
        iter = std::find(splitQuery.begin(),splitQuery.end(),"end");
    }
    std::vector<std::string> fields = std::vector<std::string>(splitQuery.begin()+1,iter);
    for(int i =0;i<fields.size();++i) {
        if(std::find(this->fieldInStruct.begin(),this->fieldInStruct.end(),split(fields[i],'=')[0]) == this->fieldInStruct.end()) {
            return false;
        }
    }
    return true;
}

//проверяет что если есть sort то он будет примерно так ... sort group end
bool AnalyseQuery::checkSort(const std::string &query) {
    std::vector<std::string> splitQuery = split(query,' ');
    if (std::find(splitQuery.begin(),splitQuery.end(),"sort") != splitQuery.end())
    {
        if (splitQuery[splitQuery.size()-3] == "sort"){
            if (std::find(this->fieldInStruct.begin(),this->fieldInStruct.end(),splitQuery[splitQuery.size()-2]) != this->fieldInStruct.end()) {
                return true;
            }
        }
        return false;
    }
    else {
        return true;
    }
}



