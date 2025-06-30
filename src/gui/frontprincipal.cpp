#include "gui/frontprincipal.h"
#include "ui_frontprincipal.h"
#include "services/visionservices.h"
#include <QThread>

frontprincipal::frontprincipal(QWidget *parent) : QMainWindow(parent), ui(new Ui::frontprincipal)
{
    ui->setupUi(this);
    m_cameraThread = new QThread();
    m_visionServices = new VisionServices();

    m_visionServices->moveToThread(m_cameraThread);

    connect(m_cameraThread, &QThread::started, m_visionServices, &VisionServices::process);
    connect(m_visionServices, &VisionServices::frameReady, this, &frontprincipal::updateCameraView);
    connect(m_visionServices, &VisionServices::finished, m_cameraThread, &QThread::quit);
    connect(m_cameraThread, &QThread::finished, m_visionServices, &QObject::deleteLater);
    connect(m_cameraThread, &QThread::finished, m_cameraThread, &QObject::deleteLater);

    m_cameraThread->start();
}

frontprincipal::~frontprincipal()
{
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