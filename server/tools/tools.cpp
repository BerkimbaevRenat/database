

#include "tools.hpp"

std::vector<std::string> split(std::string text, char delim) {
    std::string line;
    std::vector<std::string> vec;
    std::replace(text.begin(), text.end(), ',', ' ');
    text.erase(std::remove(text.begin(), text.end(), '"'), text.end());
    text.erase(std::remove(text.begin(), text.end(), ';'), text.end());
    std::stringstream ss(text);
    while (std::getline(ss, line, delim)) {
        vec.push_back(line);
    }
    vec.erase(std::remove(vec.begin(), vec.end(), ""), vec.end());
    vec.erase(std::remove(vec.begin(), vec.end(), ","), vec.end());
    return vec;
}

char* serialize(std::vector<std::string>& v, unsigned int* count) {
    unsigned int total_count = 0;

    for (int i = 0; i < v.size(); i++) {
        // cout << v[i]<< endl;
        //total_count += v[i].length() + 1;
        std::replace(v[i].begin(),v[i].end(),' ','_');
    }

    for (int i = 0; i < v.size(); i++) {
        // cout << v[i]<< endl;
        total_count += v[i].length() + 1;
    }

    char* buffer = new char[total_count];

    int idx = 0;

    for (int i = 0; i < v.size(); i++) {
        std::string s = v[i];
        for (int j = 0; j < s.size(); j++) {
            buffer[idx++] = s[j];
        }
        buffer[idx++] = 0;
    }

    *count = total_count;

    return buffer;
}

void deserialize(std::vector<std::string>& restore, char* buffer, int total_count) {
    for (int i = 0; i < total_count; i++) {
        const char* begin = &buffer[i];
        int size = 0;
        while (buffer[i++]) {
            size += 1;
        }
        restore.push_back(std::string(begin, size));
    }
}

void to_lower(std::vector<std::string>& text) {
    for (auto& word : text) {
        for (auto& c : word)
        {
            c = tolower(c);
        }
    }
}

std::string SerializeVectorsToSend(std::vector<std::string> header, std::vector<std::string> data)
{
    unsigned int total_count_headers = 0;
    char* buffer_headers = serialize(header, &total_count_headers);
    std::string headers_serialize = std::string(buffer_headers, total_count_headers);

    unsigned int total_count_data = 0;
    char* buffer_data = serialize(data, &total_count_data);
    std::string data_serialize = std::string(buffer_data, total_count_data);

    std::string result = headers_serialize + " " + data_serialize;

    return result;
}

std::vector<std::string> joinVector(std::vector<std::vector<std::string>> data) {
    std::vector<std::string> res;
    for(const auto &oneRow: data) {
        std::string buffer;
        for(const auto &oneField: oneRow) {
            buffer +=oneField+"|";
        }
        res.push_back(buffer);
    }
    return res;
}

std::vector<std::string> fromStringToVectorString(std::string data) {
    std::vector<std::string> res;
    std::vector<std::string> splitData = split(data,'|');
    for(const auto &oneValue: splitData) {
        if (!oneValue.empty()) {
            res.push_back(oneValue);
        }
    }
    return res;
}