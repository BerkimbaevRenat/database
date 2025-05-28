
#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

std::vector<std::string> split(std::string text, char delim);
char* serialize(std::vector<std::string>& v, unsigned int* count);
void deserialize(std::vector<std::string>& restore, char* buffer, int total_count);
void to_lower(std::vector<std::string>& text);
std::string SerializeVectorsToSend(std::vector<std::string> header, std::vector<std::string> data);
std::vector<std::string> joinVector(std::vector<std::vector<std::string>> data);
std::vector<std::string> fromStringToVectorString(std::string data);