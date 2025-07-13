### Índice
1. [Visão Geral](#visão-geral)
2. [Ferramentas](#ferramentas)
3. [Requisitos](#requisitos)
4. [Execução](#execução)
5. [Estrutura do Projeto](#estrutura-do-projeto)
6. [Imagens em execução](#imagens-em-execução)

---

## Visão Geral

Este projeto, HQ Translator, é uma aplicação de tradução de texto em tempo real desenvolvida em C++. Ele foi projetado para capturar frames de uma webcam, detectar textos nos frames, traduzi-los instantaneamente e, em seguida, sobrepor as traduções de volta no feed de vídeo original. A solução adota uma arquitetura robusta e multi-threaded para garantir alto desempenho.

### Como Funciona a Arquitetura Multi-threaded: Paralelismo em pipeline.

O Hq Translator opera através de um eficiente pipeline de processamento assíncrono, onde cada etapa é executada em uma thread dedicada, garantindo que operações complexas não congelem a aplicação:

Captura de Vídeo, uma thread dedicada gerencia a webcam, capturando frames continuamente e os disponibilizando para as próximas etapas.

Detecção de Texto (OCR), outra thread consome esses frames, utilizando a poderosa biblioteca Tesseract OCR para identificar e extrair textos presentes na imagem. Para otimizar o desempenho, esta etapa é executada em intervalos controlados.

Tradução Inteligente, uma terceira thread recebe os textos detectados. Ela se integra com a API LibreTranslate para realizar as traduções. Um mecanismo de cache é implementado para armazenar traduções frequentes, minimizando chamadas repetidas à API e melhorando a performance.

Exibição e Sobreposição, a thread principal da aplicação exibe o feed de vídeo da webcam. As traduções obtidas são desenhadas dinamicamente sobre o vídeo, permitindo que o usuário veja a tradução em tempo real.

## Ferramentas de Desenvolvimento

Este projeto foi construído utilizando as seguintes tecnologias e bibliotecas principais:

_C++_, a linguagem de programação principal para todo o desenvolvimento do sistema.

_OpenCV_, biblioteca de visão computacional essencial para a captura de vídeo da webcam, manipulação de frames e pré-processamento de imagens.

_Tesseract_, motor de Reconhecimento Óptico de Caracteres, utilizado para a detecção e extração de texto de imagens e frames de vídeo.

_Nlohmann_, biblioteca C++ para parseamento e serialização de dados no formato JSON, fundamental para a comunicação com a API do LibreTranslate.

_LibreTranslate_, uma API de tradução de código aberto, empregada para realizar as traduções dos textos detectados.

_Docker_, utilizado para criar um ambiente de execução consistente e isolado, simplificando as dependências.

### Ferramentas de Comunicação

Além das ferramentas citadas anteriormente, temos as ferramentas de desenvolvimento e comunicação:

_Visual Studio Code_, ambiente de Desenvolvimento Integrado (IDE) leve e poderoso, utilizado para a escrita, depuração e gerenciamento do código-fonte.

_Git_, istema de controle de versão distribuído, essencial para o rastreamento de alterações no código.

_GitHub_, plataforma de hospedagem de código-fonte baseada em Git, utilizada para o compartilhamento do repositório, colaboração em equipe, controle de versões e gerenciamento de projetos.

_WhatsApp_ utilizado para comunicação rápida e direta entre os membros da equipe.

_Discord_, plataforma de comunicação que oferece recursos como chat de texto, voz e vídeo, canais temáticos e compartilhamento de arquivos, ideal para discussões mais aprofundadas e reuniões de equipe.

---

## Requisitos

É necessário ter uma Webcam e o Docker instalado.

Caso não tenha o Docker instalado, siga o passo a passo para instalação no Linux ou via WSL:

1. Instale as dependências do Docker:

```bash
sudo apt-get update
sudo apt-get install ca-certificates curl apt-transport-https software-properties-common
```

2. Adicione a chave GPG:

```bash
sudo install -m 0755 -d /etc/apt/keyrings
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc
sudo chmod a+r /etc/apt/keyrings/docker.asc
```

3. Adicione o repositório Docker ao apt sources:

```bash
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

4. Instale o Docker:

```bash
sudo apt-get update
sudo apt-get install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

5. Adicione seu usuário ao grupo docker para evitar o uso de "sudo" (IMPORTANTE PARA EXECUÇÃO):
```bash
sudo usermod -aG docker ${USER}
```
Necessária reinicialização do computador após o passo 5.

## Execução

Clone o projeto:

```bash
git clone https://github.com/marcelohenrique15/hq-translator.git
cd hq-translator
```

Dê permissão para os scripts do projeto:

```bash
chmod +x start_docker.sh stop_docker.sh delete_docker.sh run.sh
```

Ative o Docker (pode levar alguns minutos):

```bash
./start_docker.sh
```

E após entrar no terminal bash do Docker, execute:

```bash
./run.sh
```

---

## Estrutura do Projeto

```
.
├── CMakeLists.txt                  # Configuração da Build do projeto.
├── include                         # Diretório de todas as declarações das classes utilizadas.
│   ├── capture
│   │   └── webcam_capture.h
│   ├── detection
│   │   └── text_detector.h
│   ├── overlayer
│   │   └── overlay_renderer.h
│   ├── threads
│   │   ├── detection_thread.h
│   │   ├── translate_thread.h
│   │   └── webcam_thread.h
│   └── translation
│       └── LibreTranslate.h
├── main.cpp                        # Arquivo principal do projeto.
├── README.md                       # Documentação do projeto (este arquivo).
├── run.sh                          # Script de execução do projeto.
├── src                             # Diretório da lógica de todas as classes.
│   ├── capture                  
│   │   ├── CMakeLists.txt          # Configuração da Build para webcam_capture.cpp
│   │   └── webcam_capture.cpp      # Captura de frames pela Webcam.
│   ├── detection
│   │   ├── CMakeLists.txt          # Configuração da Build para text_detector.cpp
│   │   └── text_detector.cpp       # Detecção do texto no frame gerado pela Webcam.
│   ├── overlayer
│   │   ├── CMakeLists.txt          # Configuração da Build para overlay_renderer.cpp
│   │   └── overlay_renderer.cpp    # Renderiza o texto traduzido na Webcam, por cima do texto original.
│   ├── threads
│   │   ├── CMakeLists.txt          # Configuração da Build para todas as Threads.
│   │   ├── detection_thread.cpp    # Consumidor de webcam_thread e produtor de translate_thread.
│   │   ├── translate_thread.cpp    # Consumidor de detection_thread.
│   │   └── webcam_thread.cpp       # Produtor de detection_thread.
│   └── translation
│       ├── CMakeLists.txt          # Configuração da Build de LibreTranslate.cpp
│       └── LibreTranslate.cpp      # Traduz os textos detectados pelo OCR.
└── struct
    └── struct_data.h               # Estrutura dos dados que passarão pelas threads.
```

---

## Imagens em execução
![image](https://github.com/user-attachments/assets/8e4591b0-528b-4480-bb35-b6f90bb3cb48)
