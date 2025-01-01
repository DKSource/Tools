#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <locale>

void displayCredits() {
    // Configurar o local para UTF-8
    std::setlocale(LC_ALL, "");

    // Usar caracteres simples para compatibilidade universal
    std::cout << "\033[1;34m"  // Cor azul brilhante
              << "+==================================+\n"
              << "|                                  |\n"
              << "| \033[1;33m       DK Source - Kevin\033[1;34m         |\n"
              << "|                                  |\n"
              << "+==================================+\n"
              << "\033[0m";  // Resetar cores
    std::cout << "\033[1;32mConversor de Arquivos npcscript.inf/txt!\033[0m\n\n";
}

void checkStringLength(size_t length, size_t maxLength = 2048) {
    if (length >= maxLength) {
        throw std::runtime_error("String muito longa: " + std::to_string(length));
    }
}

void convertInfToTxt(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream inf(inputFile, std::ios::binary);
    std::ofstream txt(outputFile);

    if (!inf.is_open() || !txt.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivos.");
    }

    int npcScriptCount;
    inf.read(reinterpret_cast<char*>(&npcScriptCount), sizeof(npcScriptCount));
    txt << "NpcScriptCount=" << npcScriptCount << "\n";

    for (int i = 1; i <= npcScriptCount; ++i) {
        int id;
        inf.read(reinterpret_cast<char*>(&id), sizeof(id));
        txt << "[" << i << "]\nID=" << id << "\n";

        int strLength;
        char buffer[2048];

        inf.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
        checkStringLength(strLength);
        inf.read(buffer, strLength);
        buffer[strLength] = '\0';
        txt << "OwnerID=" << buffer << "\n";

        int subjectCount;
        inf.read(reinterpret_cast<char*>(&subjectCount), sizeof(subjectCount));
        txt << "SubjectCount=" << subjectCount << "\n";

        for (int j = 1; j <= subjectCount; ++j) {
            inf.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
            checkStringLength(strLength);
            inf.read(buffer, strLength);
            buffer[strLength] = '\0';
            std::string value = buffer;
            std::replace(value.begin(), value.end(), '\n', '#');
            txt << "Subject" << j << "=" << value << "\n";
        }

        int contentCount;
        inf.read(reinterpret_cast<char*>(&contentCount), sizeof(contentCount));
        txt << "ContentCount=" << contentCount << "\n";

        for (int j = 1; j <= contentCount; ++j) {
            inf.read(reinterpret_cast<char*>(&strLength), sizeof(strLength));
            checkStringLength(strLength);
            inf.read(buffer, strLength);
            buffer[strLength] = '\0';
            std::string value = buffer;
            std::replace(value.begin(), value.end(), '\n', '#');
            txt << "Content" << j << "=" << value << "\n";
        }
    }
}

void convertTxtToInf(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream txt(inputFile);
    std::ofstream inf(outputFile, std::ios::binary);

    if (!txt.is_open() || !inf.is_open()) {
        throw std::runtime_error("Erro ao abrir arquivos.");
    }

    int npcScriptCount;
    txt.ignore(16); // Ignorar "NpcScriptCount="
    txt >> npcScriptCount;
    inf.write(reinterpret_cast<char*>(&npcScriptCount), sizeof(npcScriptCount));

    std::string line, key, value;
    char buffer[2048];

    for (int i = 1; i <= npcScriptCount; ++i) {
        txt.ignore(256, '\n'); // Ignorar "[i]"
        txt.ignore(3);         // Ignorar "ID="
        int id;
        txt >> id;
        inf.write(reinterpret_cast<char*>(&id), sizeof(id));

        txt.ignore(10); // Ignorar "OwnerID="
        std::getline(txt, value);
        checkStringLength(value.length());
        std::strcpy(buffer, value.c_str());
        int strLength = value.length();
        inf.write(reinterpret_cast<char*>(&strLength), sizeof(strLength));
        inf.write(buffer, strLength);

        txt.ignore(16); // Ignorar "SubjectCount="
        int subjectCount;
        txt >> subjectCount;
        inf.write(reinterpret_cast<char*>(&subjectCount), sizeof(subjectCount));

        for (int j = 1; j <= subjectCount; ++j) {
            txt.ignore(256, '=');
            std::getline(txt, value);
            std::replace(value.begin(), value.end(), '#', '\n');
            strLength = value.length();
            checkStringLength(strLength);
            std::strcpy(buffer, value.c_str());
            inf.write(reinterpret_cast<char*>(&strLength), sizeof(strLength));
            inf.write(buffer, strLength);
        }

        txt.ignore(16); // Ignorar "ContentCount="
        int contentCount;
        txt >> contentCount;
        inf.write(reinterpret_cast<char*>(&contentCount), sizeof(contentCount));

        for (int j = 1; j <= contentCount; ++j) {
            txt.ignore(256, '=');
            std::getline(txt, value);
            std::replace(value.begin(), value.end(), '#', '\n');
            strLength = value.length();
            checkStringLength(strLength);
            std::strcpy(buffer, value.c_str());
            inf.write(reinterpret_cast<char*>(&strLength), sizeof(strLength));
            inf.write(buffer, strLength);
        }
    }
}

int main(int argc, char* argv[]) {
    displayCredits();

    if (argc != 2) {
        std::cerr << "Uso: arraste um arquivo .inf ou .txt para o executável.\n";
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile;

    try {
        if (inputFile.substr(inputFile.find_last_of(".") + 1) == "inf") {
            outputFile = inputFile.substr(0, inputFile.find_last_of(".")) + ".txt";
            convertInfToTxt(inputFile, outputFile);
            std::cout << "Conversão de INF para TXT concluída: " << outputFile << "\n";
        } else if (inputFile.substr(inputFile.find_last_of(".") + 1) == "txt") {
            outputFile = inputFile.substr(0, inputFile.find_last_of(".")) + ".inf";
            convertTxtToInf(inputFile, outputFile);
            std::cout << "Conversão de TXT para INF concluída: " << outputFile << "\n";
        } else {
            std::cerr << "Arquivo não suportado. Use arquivos .inf ou .txt.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
