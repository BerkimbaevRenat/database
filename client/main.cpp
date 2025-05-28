//refer ClientCreation for more info

#include <iostream>
#include <locale>
#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pcap/socket.h>


std::vector<std::string> split(std::string text, char delim) {
    std::string line;
    std::vector<std::string> vec;
    text.erase(std::remove(text.begin(), text.end(), '"'), text.end());
    text.erase(std::remove(text.begin(), text.end(), ';'), text.end());
    std::stringstream ss(text);
    while (std::getline(ss, line, delim)) {
        vec.push_back(line);
    }
    return vec;
}

char* serialize(std::vector<std::string>& v, unsigned int* count) {
    unsigned int total_count = 0;

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

void deserialize(std::vector<std::string>& restore,const char* buffer, int total_count) {
    for (int i = 0; i < total_count;) {
        const char* begin = &buffer[i];
        int size = 0;
        while (buffer[i++]) {
            size += 1;
        }
        restore.push_back(std::string(begin, size));
    }
}


int main() {

    std::locale current_locale("");
    std::locale::global(current_locale);
    //socket creation
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << std::endl;
        return 0;
    }
    else {
        std::cout << "socket is OK!" << std::endl;
    }

    //connection to server
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(55557);
    if (connect(clientSocket, (struct sockaddr*)&clientService, sizeof(clientService)) == -1) {
        std::cout << "Client: connect() - Failed to connect: " << std::endl;
        return 0;
    }
    else {
        std::cout << "Client: Connect() is OK!" << std::endl;
        std::cout << "Client: Can start sending and receiving data..." << std::endl;
    }

    while (1) {
        //sending data
        char buffer[200];
        printf("Enter the message: ");
        std::cin.getline(buffer, 200);
        if (std::string(buffer) == "quit") {
            return 0;
        }
        int sbyteCount = send(clientSocket, buffer, 200, 0);
        if (sbyteCount == -1) {
            std::cout << "Server send error: " << std::endl;
            return -1;
        }
        else {
            std::cout << "Server: sent" << sbyteCount << std::endl;
        }

        char receiveBuffer[1024];
        int byteCount = recv(clientSocket, receiveBuffer, 1024, 0);

        if (byteCount < 0) {
            std::cout << "Server recv error: " << std::endl;
            return 0;
        }
        else {

            std::string getData = std::string(receiveBuffer, byteCount);
            std::vector<std::string> part = split(getData, ' ');
            if (part.size() > 2) {
                std::cout << getData << std::endl;
            }

            else {

                std::vector<std::string> header;
                deserialize(header, part[0].c_str(), part[0].size());

                std::vector<std::string> data;
                deserialize(data, part[1].c_str(), part[1].size());

                for (const auto& val : header)
                    std::cout << std::setw(20) << val;
                std::cout << '\n';

                for (size_t i = 0; i < data.size(); ++i) {
                    for (const auto& val : split(data[i], '|')) {
                        std::cout << std::setw(20) << std::fixed
                                  << std::setprecision(2) << val;
                    }
                    std::cout << '\n';
                }
            }

        }





    }
}
