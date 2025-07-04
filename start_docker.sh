#!/bin/bash
# --- Configurações ---
# Nome do serviço do seu aplicativo no docker-compose.yml.
# O nome da imagem criada será 'nome_do_diretorio_do_projeto_nome_do_servico'.
# Por exemplo, se seu diretório for 'hq-translator' e o serviço 'hq-translator', a imagem será 'hq-translator_hq-translator'.
# Ajuste conforme necessário se você usa um nome de diretório diferente.
APP_SERVICE_NAME="hq-translator"

# --- Lógica de Inicialização ---
echo "--- Iniciando o projeto HQ-Translator ---"

# Verifica se a imagem do seu aplicativo já existe localmente
# Usamos 'docker images -q' para listar apenas os IDs das imagens
# e filtramos pelo nome do serviço para encontrar a imagem construída pelo Compose.
export DISPLAY=${DISPLAY:-:0}
if [[ -z "$(docker images -q "$(basename "$PWD")_${APP_SERVICE_NAME}")" ]]; then
  echo "Imagem do '$APP_SERVICE_NAME' não encontrada. Construindo e iniciando serviços (pode levar um tempo na primeira vez)..."
  docker compose up --build -d # Usa --build para construir a imagem e -d para rodar em segundo plano
  docker exec -it hq_translator_app_container bash
else
  echo "Imagem do '$APP_SERVICE_NAME' encontrada. Iniciando serviços sem rebuild..."
  docker compose up -d # Inicia os serviços em segundo plano, usando a imagem existente
  docker exec -it hq_translator_app_container bash
fi