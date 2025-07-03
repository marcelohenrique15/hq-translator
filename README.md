### Índice
1. [Visão Geral](#visão-geral)
2. [Ferramentas](#ferramentas)
2. [Requisitos](#requisitos)
3. [Execução](#execução)
4. [Estrutura do Projeto](#estrutura-do-projeto)

---

## Visão Geral

Este projeto, HQ Translator, é uma aplicação de tradução de texto em tempo real desenvolvida em C++. Ele foi projetado para capturar frames de uma webcam, detectar textos nos frames, traduzi-los instantaneamente e, em seguida, sobrepor as traduções de volta no feed de vídeo original. A solução adota uma arquitetura robusta e multi-threaded para garantir alto desempenho.

### Como Funciona a Arquitetura Multi-threaded: Paralelismo em pipeline.

O Hq Translator opera através de um eficiente pipeline de processamento assíncrono, onde cada etapa é executada em uma thread dedicada, garantindo que operações complexas não congelem a aplicação:

Captura de Vídeo, uma thread dedicada gerencia a webcam, capturando frames continuamente e os disponibilizando para as próximas etapas.

Detecção de Texto (OCR), outra thread consome esses frames, utilizando a poderosa biblioteca Tesseract OCR para identificar e extrair textos presentes na imagem. Para otimizar o desempenho, esta etapa é executada em intervalos controlados.

Tradução Inteligente, uma terceira thread recebe os textos detectados. Ela se integra com a API LibreTranslate para realizar as traduções. Um mecanismo de cache é implementado para armazenar traduções frequentes, minimizando chamadas repetidas à API e melhorando a performance.

Exibição e Sobreposição, a thread principal da aplicação exibe o feed de vídeo da webcam. As traduções obtidas são desenhadas dinamicamente sobre o vídeo, permitindo que o usuário veja a tradução em tempo real.
---

## Ferramentas 

Este projeto foi construído utilizando as seguintes tecnologias e bibliotecas principais:

*C++*, a linguagem de programação principal para todo o desenvolvimento do sistema.

*OpenCV*, biblioteca de visão computacional essencial para a captura de vídeo da webcam, manipulação de frames e pré-processamento de imagens.

*Tesseract*, motor de Reconhecimento Óptico de Caracteres, utilizado para a detecção e extração de texto de imagens e frames de vídeo.

*Nlohmann* biblioteca C++ para parseamento e serialização de dados no formato JSON, fundamental para a comunicação com a API do LibreTranslate.

*LibreTranslate*, uma API de tradução de código aberto, empregada para realizar as traduções dos textos detectados.

### Ferramentas de Desenvolvimento e Comunicação

Além das ferramentas citadas anteriormente, temos as ferramentas de desenvolvimento e comunicação:

*VS Code*, ambiente de Desenvolvimento Integrado (IDE) leve e poderoso, utilizado para a escrita, depuração e gerenciamento do código-fonte.

*Git*, istema de controle de versão distribuído, essencial para o rastreamento de alterações no código.

*GitHub*, plataforma de hospedagem de código-fonte baseada em Git, utilizada para o compartilhamento do repositório, colaboração em equipe, controle de versões e gerenciamento de projetos.

*WhatsApp*, utilizado para comunicação rápida e direta entre os membros da equipe.

*Discord*, plataforma de comunicação que oferece recursos como chat de texto, voz e vídeo, canais temáticos e compartilhamento de arquivos, ideal para discussões mais aprofundadas e reuniões de equipe.

## Requisitos

É necessário executar o programa no sistema operacional Ubuntu 22.04, além disso, será necessário instalar as bibliotecas abaixo, ter uma Webcam e possuir o Docker instalado.

### OpenCV

Essa biblioteca é responsável por implementar as ferramentas de visão computacional que serão utilizadas.

Utilize os seguintes comandos:

```bash
sudo apt-get update
sudo apt install libopencv-dev
```

Confirme a instalação com o comando:

```bash
dpkg -l libopencv-dev
```
Deverá ter como resposta a versão do OpenCV que foi instalada.

### Leptonica e Tesseract
O Tesseract é a API responsável pela IA que irá detectar textos mostrados nas imagens, e no caso deste projeto, detectará através da Webcam.

Para instalar o Leptonica (dependência do Tesseract), utilize o seguinte comando:

```bash
sudo apt install libleptonica-dev
```

Agora, instale bibliotecas que servirão para ler diferentes tipos de imagem:

```bash
sudo apt-get install libpng-dev libjpeg-dev libtiff-dev libgif-dev libwebp-dev libopenjp2-7-dev zlib1g-dev
```

Para instalar o Tesseract, utilize o seguinte comando:

```bash
cd ~/Desktop
wget https://github.com/tesseract-ocr/tesseract/archive/refs/tags/5.3.0.tar.gz
tar -xzvf 5.3.0.tar.gz 
cd tesseract-5.3.0/
mkdir build
cd build
cmake ..
make -j `nproc`
sudo make install
```

Agora instale o modelo treinado para detectar textos em inglês:

```bash
wget https://raw.githubusercontent.com/tesseract-ocr/tessdata_best/main/eng.traineddata
sudo mv *.traineddata /usr/local/share/tessdata
```

### Nlohmann
Essa biblioteca é responsável por permitir que o programa trabalhe utilizando Json, assim será possível fazer requisições para tradução do texto detectado pelo Tesseract.

Para instalar a biblioteca Nlohmann, utilize:

```bash
sudo apt-get -y install nlohmann-json3-dev
```

---

## Execução

Após instalar todas as dependências, clone este repositório:

```bash
git clone https://github.com/marcelohenrique15/hq-translator.git
cd hq-translator
```

Ative o docker para tradução local:

```bash
sudo docker run -d -p 5001:5000 libretranslate/libretranslate
```

Então execute:

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