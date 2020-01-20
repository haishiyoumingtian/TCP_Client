#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void msghandle(QByteArray array);

private slots:

    void on_transfer_btn_clicked();

    void on_close_btn_clicked();

    void on_connect_btn_clicked();

    void on_quit_btn_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket  *tcpSocket;//通信套接字

    int connected;
};

#endif // MAINWINDOW_H
