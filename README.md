### Índice
1. [Visão Geral](#visão-geral)
2. [Requisitos](#requisitos)
3. [Execução](#execução)

## Visão Geral

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

### Nlohmann
Essa biblioteca é responsável por permitir que o programa trabalhe utilizando Json, assim será possível fazer requisições para tradução do texto detectado pelo Tesseract.

Para instalar a biblioteca Nlohmann, utilize:

```bash
sudo apt-get -y install nlohmann-json3-dev
```
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