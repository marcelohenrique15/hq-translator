#pragma once

#include <QMainWindow>
#include <QImage>

class QThread;
class VisionServices;

namespace Ui {
class frontprincipal;
}

class frontprincipal : public QMainWindow
{
    Q_OBJECT

    public:
        explicit frontprincipal(QWidget *parent = nullptr);
        ~frontprincipal();

    public slots:
        void updateCameraView(const QImage &image);

    private:
        Ui::frontprincipal *ui;
        QThread * m_cameraThread;
        VisionServices* m_visionServices;
};