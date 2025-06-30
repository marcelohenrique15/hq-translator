#include "gui/frontprincipal.h"
#include <QApplication>

int main(int argc, char *argv[]){

    //cria a aplicação qt
    QApplication a(argc, argv);

    //cria a janela principal
    frontprincipal w;

    //mostra a janela
    w.show();

    return a.exec();

}