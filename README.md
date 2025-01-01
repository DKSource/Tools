## Conversor de Arquivos .inf/txt do mmorpg DarkEden

Este projeto é um utilitário simples para converter arquivos no formato `.inf` para `.txt` e vice-versa. Ele foi projetado para ser compatível tanto com **Windows** quanto com **Linux**, priorizando um código simples e limpo.
- Converte arquivos `.inf` em `.txt` e vice-versa.
- Detecta automaticamente o tipo de arquivo com base na extensão.

---

## 🛠️ Compilação

Este programa pode ser compilado tanto no **Windows** quanto no **Linux**.

### Windows
- Utilize o Dev-C++ ou qualquer compilador C++ com suporte ao padrão C++17.
- Certifique-se de que o terminal do Windows tem suporte a ANSI habilitado (Windows 10 ou superior).

### Linux
- Certifique-se de ter o **clang** ou **gcc** instalado.
- Comando de compilação:
  ```sh
  clang++ -o string_tool string_tool.cpp -std=c++17 -static
