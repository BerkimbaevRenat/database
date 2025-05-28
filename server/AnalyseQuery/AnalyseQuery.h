//
// Created by mars on 21.05.2024.
//

#ifndef SERVER_ANALYSEQUERY_H
#define SERVER_ANALYSEQUERY_H

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "../Schedule/Schedule.h"
#include "../tools/tools.hpp"
class AnalyseQuery {
public:
    AnalyseQuery(Schedule* schedule) {this->schedule = schedule;};

    std::string analyse(const std::string& query);
private:

    //analyse any query
    std::string analyseSelect(const std::string& query);
    std::string analyseInsert(const std::string& query);
    std::string analyseRemove(const std::string& query);

    //get-set
    std::string getCommand(const std::string& query);
    std::map<std::string,std::string> getField(const std::string &query);


    //check correct
    bool checkQuery(const std::string& query); //complete check
    bool checkCorrectCommand(const std::string& query);
    bool checkCorrectFinish(const std::string& query);
    bool checkCorrectField(const std::string& query);
    bool checkSort(const std::string& query);


    std::vector<std::string> allowCommand = {"select","insert","remove"};
    std::vector<std::string> fieldInStruct = {"date","numberlesson","classroom","subject","teacher","group"};
    std::vector<std::string> headerToSend = {"teacher","subject","date","group","numberlesson","classroom"};
    //std::map<std::string,std::string> = ["error_query":"dasd"];
    Schedule* schedule;
};
#endif //SERVER_ANALYSEQUERY_H
