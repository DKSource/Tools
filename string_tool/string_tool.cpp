#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdint> // Necessário para uint32_t

#ifdef _WIN32
#include <windows.h>
#endif

// Função para configurar o console
void configureConsole() {
#ifdef _WIN32
    // Configura o console do Windows para usar UTF-8 e habilita o suporte a cores ANSI
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hConsole, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hConsole, mode);
#endif
}

// Função para exibir os créditos com cores
void displayCredits() {
    std::cout << "\033[1;34m"  // Cor azul brilhante
              << "╔══════════════════════════════════╗\n"
              << "║                                  ║\n"
              << "║ \033[1;33m       DK Source - Kevin\033[1;34m         ║\n"
              << "║                                  ║\n"
              << "╚══════════════════════════════════╝\n"
              << "\033[0m";  // Resetar cores
    std::cout << "\033[1;32mConversor de Arquivos string.inf/txt!\033[0m\n\n";
}

void convertInfToTxt(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    std::ofstream out(outputFile);

    if (!in || !out) {
        std::cerr << u8"\033[1;31mErro ao abrir os arquivos.\033[0m\n";
        return;
    }

    uint32_t stringCount;
    in.read(reinterpret_cast<char*>(&stringCount), sizeof(stringCount));

    for (uint32_t i = 0; i < stringCount; ++i) {
        uint32_t strLength;
        in.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));

        std::vector<char> buffer(strLength + 1);
        in.read(buffer.data(), strLength);
        buffer[strLength] = '\0'; // Terminação

        std::string line(buffer.data());
        for (char& ch : line) {
            if (ch == '\n') ch = '\1';
        }

        out << line << '\n';
    }

    std::cout << u8"\033[1;32mConversão para TXT concluída:\033[0m " << outputFile << "\n";
}

void convertTxtToInf(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile);
    std::ofstream out(outputFile, std::ios::binary);

    if (!in || !out) {
        std::cerr << u8"\033[1;31mErro ao abrir os arquivos.\033[0m\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(in, line)) {
        for (char& ch : line) {
            if (ch == '\1') ch = '\n';
        }
        lines.push_back(line);
    }

    uint32_t stringCount = lines.size();
    out.write(reinterpret_cast<char*>(&stringCount), sizeof(stringCount));

    for (const auto& str : lines) {
        uint32_t strLength = str.length();
        out.write(reinterpret_cast<char*>(&strLength), sizeof(strLength));
        out.write(str.c_str(), strLength);
    }

    std::cout << u8"\033[1;32mConversão para INF concluída:\033[0m " << outputFile << "\n";
}

int main(int argc, char* argv[]) {
    configureConsole();
    displayCredits();

    if (argc < 2) {
        std::cerr << u8"\033[1;31mUso: arraste um arquivo string.inf/txt para o executável.\033[0m\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile;

    if (inputFile.find(".inf") != std::string::npos) {
        outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".txt";
        convertInfToTxt(inputFile, outputFile);
    } else if (inputFile.find(".txt") != std::string::npos) {
        outputFile = inputFile.substr(0, inputFile.find_last_of('.')) + ".inf";
        convertTxtToInf(inputFile, outputFile);
    } else {
        std::cerr << u8"\033[1;31mArquivo não suportado. Use .inf ou .txt\033[0m\n";
        return 1;
    }

    return 0;
}

