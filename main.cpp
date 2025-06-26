#include <QApplication>
#include "gui/frontprincipal.h"

int main(int argc, char *argv[])
{
    // 1. Cria o objeto da aplicação Qt
    QApplication a(argc, argv);

    // 2. Cria a instância da janela principal
    frontprincipal w;

    // 3. Mostra a janela
    w.show();

    // 4. Inicia o loop de eventos
    return a.exec();
}