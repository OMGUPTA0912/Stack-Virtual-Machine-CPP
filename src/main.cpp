#include <iostream>
#include <fstream>
#include <sstream>
#include "Lexer.h"
#include "Parser.h"
#include "Compiler.h"
#include "VM.h"

void run(const std::string& source, bool debugBytes = false) {
    try {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.scanTokens();
        
        Parser parser(tokens);
        std::vector<std::unique_ptr<Stmt>> ast = parser.parse();
        
        Compiler compiler;
        std::vector<uint8_t> bytecode = compiler.compile(ast);
        
        if (debugBytes) {
            std::cout << "--- Compiled Bytecode Output ---\n";
            for (size_t i = 0; i < bytecode.size(); i++) {
                std::cout << "0x" << std::hex << (int)bytecode[i] << std::dec << " ";
            }
            std::cout << "\n--------------------------------\n";
        }
        
        VM vm;
        vm.interpret(bytecode);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void runREPL(bool debugBytes = false) {
    std::string line;
    std::cout << "Custom Stack VM REPL (type 'exit' to quit)\n";
    std::cout << "Note: REPL executes independent scripts per line.\n";
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line == "exit") {
            break;
        }
        if (line.empty()) continue;
        
        run(line, debugBytes);
    }
}

void runFile(const std::string& path, bool debugBytes = false) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        return;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    run(buffer.str(), debugBytes);
}

int main(int argc, char* argv[]) {
    bool debug = false;
    std::string filePath = "";

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--debug") debug = true;
        else filePath = arg;
    }

    if (filePath != "") {
        runFile(filePath, debug);
    } else {
        runREPL(debug);
    }
    
    return 0;
}
