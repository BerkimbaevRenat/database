#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>
#include <string>
#include <signal.h>
#include <locale>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pcap/socket.h>

#include "Schedule/Schedule.h"
#include "AnalyseQuery/AnalyseQuery.h"
Schedule schedule;
AnalyseQuery analyseQuery(&schedule);
/*
int main() {

    std::string query;
    std::string res;
    while(1) {
        std::getline(std::cin, query);
        if(!query.empty()) {
            if(query == "exit") {
                return 1;
            }
            res = analyseQuery.analyse(query);
            std::cout << res << std::endl;
        }
    }
}

*/

/*
    Функция, которая работает в паралельном потоке, обрабатывает обращения клиента по идентификатору сокета
*/
void ChatWithUser(const int acceptSocket) {

    if (acceptSocket == INVALID_SOCKET) {
        std::cout << "accept failed: " << std::endl;
        return;
    }
    else {
        std::cout << "accept() is OK!" << std::endl;
    }

    while (1) {
        // получаем данные
        char receiveBuffer[200];
        size_t rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
        if (rbyteCount == 0) {
            std::cout << "Server recv error: "  << std::endl;
            return;
        }
        else {
            std::cout << "Received data: " << receiveBuffer << std::endl;
        }

        if (std::string(receiveBuffer) == "quit") {
            return;
        }
        std::string result = "";
        if (std::string(receiveBuffer) != "") {
            // обрабатываем полученный запрос
            result = analyseQuery.analyse(std::string(receiveBuffer));
        }


        // отправляем данные
        size_t sbyteCount = send(acceptSocket, result.c_str(), result.size(), 0);
        if (sbyteCount == -1) {
            std::cout << "Server send error: "  << std::endl;
            return;
        }
        else {
            std::cout << "Server: sent" << sbyteCount << std::endl;
        }
    }
}

void waitExit(int signal) {
    std::cout <<  signal ;
    exit(signal);
}



int main() {
    signal(SIGINT, waitExit);
    signal(SIGTERM, waitExit);
    std::locale current_locale("");
    std::locale::global(current_locale);

    // Инициализация данных
    // создание серверного сокета
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    //проверяем успешное создание сокета
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error at socket():" << std::endl;
        return 0;
    }
    else {
        std::cout << "socket is OK!" << std::endl;
    }

    // привязываем сокет к адресу и порту
    sockaddr_in serviceParameters;
    serviceParameters.sin_family = AF_INET;
    serviceParameters.sin_addr.s_addr = inet_addr("127.0.0.1");
    serviceParameters.sin_port = htons(55557);


    if (bind(serverSocket, (struct sockaddr*)&serviceParameters, sizeof(serviceParameters)) == -1) {
        std::cout << "bind() failed: " << std::endl;
        close(serverSocket);
        return 0;
    }
    else {
        std::cout << "bind() is OK!" << std::endl;
    }

    // слушаем входящие соединения
    if (listen(serverSocket, 1) == -1) {
        std::cout << "listen(): Error listening on socket: " << std::endl;
    }
    else {
        std::cout << "listen() is OK!, I'm waiting for new connections..." << std::endl;
    }

    // создаем сокет клиента и инициализируем соединение
    SOCKET acceptSocket;
    std::vector<std::thread> listConnections;
    try {

        while (1) {
            // создаем новый поток для нового клиента
            acceptSocket = accept(serverSocket, NULL, NULL);
            std::thread th(ChatWithUser, acceptSocket);
            listConnections.push_back(std::move(th));
        }

        for (int i = 0; i < listConnections.size(); ++i)
        {
            // ожидаем завершения всех потоков
            listConnections[i].join();
        }
    }
    catch(...) {
        return -1;
    }
}

/// 0(institut),1(fac),Заведующий(2) Профессор(3) доцент(4) Старший_преподаватель(5) преподаватель(6) ассистент(7)


