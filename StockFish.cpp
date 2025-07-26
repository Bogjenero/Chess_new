#include "StockFish.h"

#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string>


StockFish::StockFish() {

    #ifdef _WIN32
        stockFish = popen("stockfish.exe", "r");
    #elif __linux__
        if (pipe(toStockfish) == -1 || pipe(fromStockfish) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            // child process - Stockfish

            
            dup2(toStockfish[0], STDIN_FILENO);
            dup2(fromStockfish[1], STDOUT_FILENO);

            // Zatvori nepotrebne pipe-ove
            close(toStockfish[1]);
            close(fromStockfish[0]);

            execl("./StockFish/stockfish/stockfish-ubuntu-x86-64-avx2", "stockfish-ubuntu", nullptr);
            perror("execl failed");
            exit(EXIT_FAILURE);
        } else {
            // parent process
            close(toStockfish[0]);   // zatvori čitanje pipea za slanje Stockfishu
            close(fromStockfish[1]); // zatvori pisanje pipea za čitanje od Stockfisha              
            
            sendCommand("uci");

            std::string response;
            do {
                response = getResponse();
            } while (response.find("uciok") == std::string::npos);
                   
            sendCommand("isready");
            do {
                response = getResponse();
            } while (response.find("readyok") == std::string::npos);
        }
    #endif
}


void StockFish::sendCommand(const std::string& command) {
    #ifdef _WIN32
        fprintf(stockFish, "%s\n", command.c_str());
        fflush(stockFish);
    #elif __linux__
        write(toStockfish[1], command.c_str(), command.size());
        write(toStockfish[1], "\n", 1); // dodaj novi red
    #endif
}
std::string StockFish::getResponse() {
    std::string response;
    char buffer[256];
    ssize_t bytesRead;
    
    #ifdef _WIN32
        while (fgets(buffer, sizeof(buffer), stockFish) != nullptr) {
            response += buffer;
        }
    #elif __linux__
      fd_set readfds;
        struct timeval timeout;

        FD_ZERO(&readfds);
        FD_SET(fromStockfish[0], &readfds);

        timeout.tv_sec = 1;         // maksimalno čekaj 1 sekundu
        timeout.tv_usec = 0;

        int ready = select(fromStockfish[0] + 1, &readfds, nullptr, nullptr, &timeout);
        if (ready > 0 && FD_ISSET(fromStockfish[0], &readfds)) {
            bytesRead = read(fromStockfish[0], buffer, sizeof(buffer) - 1);
            if (bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
            }
        }
    #endif
    
    return response;
}


std::string StockFish::getBestMove(const std::string& fenPosition) {

    sendCommand("position fen " + fenPosition);
    
    sendCommand("go depth 10");  

    std::string response;
    do {
        response = getResponse();
    } while (response.find("bestmove") == std::string::npos);
    

    size_t pos = response.find("bestmove ") + 9;
    return response.substr(pos, 4);  
}
