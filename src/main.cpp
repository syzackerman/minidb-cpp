#include "Database.h"
#include <iostream>
#include <string>
#include <vector>
#include "Parser.h"
#include "Executor.h"

int main(){
    Database db;
    db.loadAll();
    Executor executor(db);

    std::string line;

    std::cout << "MiniDB started. Type EXIT to quit." << std::endl;

    while(true){
        std::cout << "minidb> ";
        std::getline(std::cin, line);
        if(line == "EXIT"){
            if(db.hasActiveTransaction()){
                std::cout << "Error: transaction in progress. Use COMMIT or ROLLBACK before EXIT." << std::endl;
                continue;
            }

            db.saveAll();
            break;
        }

        std::vector<std::string> tokens = Parser::tokenize(line);
        executor.execute(tokens);

    }
    return 0;
}