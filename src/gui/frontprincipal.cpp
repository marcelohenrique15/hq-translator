#include "gui/frontprincipal.h"
#include "ui_frontprincipal.h"
#include "services/visionservices.h"
#include <QThread>

frontprincipal::frontprincipal(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frontprincipal)
{
    ui->setupUi(this);

        // --- INÍCIO DA CONFIGURAÇÃO DA CÂMERA ---
    m_cameraThread = new QThread();
    m_visionServices = new VisionServices();

    // Move o worker para a thread. Ele viverá e executará lá.
    m_visionServices->moveToThread(m_cameraThread);

    // Conecta os sinais e slots para a mágica acontecer
    // 1. Quando a thread iniciar, comece o processamento da câmera
    connect(m_cameraThread, &QThread::started, m_visionServices, &VisionServices::process);
    // 2. Quando o worker emitir um frame, chame o nosso método para atualizar a tela
    connect(m_visionServices, &VisionServices::frameReady, this, &frontprincipal::updateCameraView);
    // 3. Quando o worker terminar, agende a thread para fechar de forma segura
    connect(m_visionServices, &VisionServices::finished, m_cameraThread, &QThread::quit);
    // 4. Quando a thread fechar, delete o worker e a thread para não vazar memória
    connect(m_cameraThread, &QThread::finished, m_visionServices, &QObject::deleteLater);
    connect(m_cameraThread, &QThread::finished, m_cameraThread, &QObject::deleteLater);

    // Inicia a thread!
    m_cameraThread->start();
    // --- FIM DA CONFIGURAÇÃO DA CÂMERA ---
}

frontprincipal::~frontprincipal()
{
    // Garante que a thread seja finalizada corretamente quando a janela fechar
    if(m_cameraThread && m_cameraThread->isRunning()) {
        m_visionServices->stop(); // Avisa ao loop para parar
        m_cameraThread->quit();   // Pede para a thread sair do loop de eventos
        m_cameraThread->wait();   // Espera a thread terminar de fato
    }
    delete ui;
}

void frontprincipal::updateCameraView(const QImage &image)
{
    ui->cameraView->setPixmap(QPixmap::fromImage(image));
}
