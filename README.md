### Índice
1. [Visão Geral](#visão-geral)
2. [Requisitos](#requisitos)
3. [Execução](#execução)
4. [Estrutura do Projeto](#estrutura-do-projeto)

---

## Visão Geral

Escreva Aqui.

---

## Requisitos

É necessário executar o programa no sistema operacional Ubuntu 22.04, além disso, será necessário instalar as bibliotecas abaixo e ter uma Webcam.

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

Então execute:

```bash
./run.sh
```

---

## Estrutura do Projeto

```
.
├── CMakeLists.txt                    # Arquivo que configura o Build da raiz do projeto
├── include                           # Pasta com todas as assinaturas das classes
│   ├── capture
│   │   └── webcam_capture.h           
│   ├── detection
│   │   └── text_detector.h            
│   ├── overlayer
│   │   └── overlay_renderer.h
│   └── translation
│       └── translate.h
├── main.cpp                          # Lógica principal do programa
├── README.md                         # Documentação do projeto                 
├── run.sh                            # Script para buildar e executar o programa
└── src                               # Pasta com todas classes do projeto
    ├── capture
    │   ├── CMakeLists.txt            # Arquivo que configura a build de webcam_capture.cpp
    │   └── webcam_capture.cpp        # Responsável pela configuração da Webcam
    ├── detection
    │   ├── CMakeLists.txt            # Arquivo que configura a build de text_detector.cpp
    │   └── text_detector.cpp         # Responsável pela configuração do Tesseract e sua detecção de texto
    ├── overlayer
    │   ├── CMakeLists.txt            # Arquivo que configura a build de overlay_renderer.cpp
    │   └── overlay_renderer.cpp      # Responsável por renderizar a tradução do texto na Webcam
    └── translation
        ├── CMakeLists.txt            # Arquivo que configura a build de translate.cpp
        └── translate.cpp             # Responsável por traduzir o texto detectado pelo Tesseract
```