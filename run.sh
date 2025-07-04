#!/bin/bash

# Define o nome do executável principal do seu projeto C++
# Ajuste 'main' para o nome que você definiu no seu CMakeLists.txt se for diferente
EXECUTABLE_NAME="main"

echo -e "\n--- Iniciando Compilação e Execução do Projeto ---\n"

# 1. Verifica e cria a pasta 'build' se necessário
if [ -d "build" ]; then
  echo "Pasta 'build' já existe."
else
  echo "Criando pasta 'build'..."
  mkdir build
fi

# 2. Navega para a pasta 'build'
# O '|| { ...; exit 1; }' faz o script parar se o 'cd' falhar
cd build || { echo "Erro: Não foi possível mudar para o diretório 'build'."; exit 1; }

# 3. Configura o projeto CMake
echo "Configurando CMake..."
# O '|| { ...; exit 1; }' faz o script parar se o 'cmake' falhar
cmake .. || { echo "Erro: Configuração do CMake falhou."; exit 1; }

# 4. Compila o projeto
echo "Compilando com make..."
# O '|| { ...; exit 1; }' faz o script parar se o 'make' falhar
make -j$(nproc) || { echo "Erro: Compilação com make falhou. Verifique os logs acima."; exit 1; }

# 5. Verifica se o executável foi criado e o executa
if [ -f "$EXECUTABLE_NAME" ]; then
  echo -e "\nCompilação concluída com sucesso! Executando o projeto...\n"
  ./$EXECUTABLE_NAME # Executa o executável principal dentro da pasta 'build'
else
  echo -e "\nERRO CRÍTICO: Executável '$EXECUTABLE_NAME' não encontrado em './build/'. A compilação *deve* ter falhado apesar de ter passado pelos testes anteriores.\n"
  exit 1 # Sai com código de erro
fi

# Não precisa de 'cd ..' aqui, pois o script vai terminar ou o programa vai rodar.