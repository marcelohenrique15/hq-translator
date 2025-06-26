#ifndef FRONTPRINCIPAL_H
#define FRONTPRINCIPAL_H

#include <QMainWindow>
#include <QImage>

class QThread;
class CameraWorker;

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
        CameraWorker* m_cameraWorker;
};

#endif // FRONTPRINCIPAL_H
