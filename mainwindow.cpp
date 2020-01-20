#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDateTime"
#include "QHostAddress"
#include "QSystemTrayIcon"
//获取当前时间
QTime current_time = QTime::currentTime();
//构造函数
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcpSocket = NULL;
    connected=0;

    //分配空间，指定父对象
    tcpSocket = new QTcpSocket(this);

    //设置标题栏以及图标
    setWindowTitle("Chat");
    setWindowIcon(QIcon("icon.ico"));

    //连接成功的连接函数
    connect(tcpSocket,&QTcpSocket::connected,[=](){
        connected = 1;
        ui->connoutput->insertPlainText("connect scceed!");
        ui->connoutput->insertPlainText("\n");
        QString name = ui->lineEditUser->text();
        tcpSocket->write(name.toUtf8().data());
    });
    //收到消息的信号的连接函数
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
        //获取对方发送的内容
        QByteArray array = tcpSocket->readAll();
        this->msghandle(array);
    });
}
//析构函数
MainWindow::~MainWindow()
{
    delete ui;
}
//发送按钮
void MainWindow::on_transfer_btn_clicked()
{
    //获取输入框内的信息
    QString str;
    str = ui->input->toPlainText();
    //如果未连接则提示先连接
    if(connected==0){
        ui->connoutput->insertPlainText("Please login first.");
        ui->connoutput->insertPlainText("\n");
        ui->connoutput->moveCursor(QTextCursor::End);
    }
    else{
        if(str==""){
            //没有输入信息则什么都不干
        }
        else{
            str=ui->lineEditUser->text()+":"+str;
            tcpSocket->write(str.toUtf8().data()); //发送文本框信息
        }
        //清空文本框
        ui->input->clear();
    }
}
//关闭按钮
void MainWindow::on_close_btn_clicked()
{
    //若未连接则直接关闭程序
    if(connected == 0){
    }
    //若已连接则关闭程序之前先断开连接
    else{
        QString str = "0";
        tcpSocket->write(str.toUtf8().data()); //发送文本框信息
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
    close();
}
//连接按钮
void MainWindow::on_connect_btn_clicked()
{
    //获取服务器的ip和端口
    QString ip = ui->lineEditIP->text();
    qint16 port = ui->lineEditPort->text().toInt();
    //若已连接，输出提示信息
    if(connected == 1){
        ui->connoutput->insertPlainText("You are already logged in.");
        ui->connoutput->insertPlainText("\n");
        ui->connoutput->moveCursor(QTextCursor::End);
    }
    //若未连接，连接后输出提示信息
    else{
        ui->connoutput->insertPlainText("connect with:"+ip);
        ui->connoutput->insertPlainText("   port:"+ui->lineEditPort->text());
        ui->connoutput->insertPlainText("\n");
        ui->connoutput->moveCursor(QTextCursor::End);

        //主动和服务器建立连接
        tcpSocket->connectToHost(QHostAddress(ip),port);
    }
}
//断开连接按钮
void MainWindow::on_quit_btn_clicked()
{
    //主动和客户端端口断开连接
    QString str = "0";
    tcpSocket->write(str.toUtf8().data()); //发送文本框信息
    tcpSocket->disconnectFromHost();
    connected=0;
    ui->connoutput->insertPlainText("You have exited from the server.");
    ui->connoutput->insertPlainText("\n");
    ui->connoutput->moveCursor(QTextCursor::End);
}
//清屏
void MainWindow::on_pushButton_clicked()
{
    ui->output->clear();
    ui->connoutput->clear();
}
//消息处理函数
void MainWindow::msghandle(QByteArray array){
    int i;
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");

    for(i=0;i<strlen(array);i++){
        if(array[i]==':'){
            break;
        }
    }
    QString name = array.mid(0,i);
    //追加到编辑区中
    ui->output->insertPlainText(current_date+" " + name+":");
    ui->output->insertPlainText("\n");

    ui->output->insertPlainText(array.mid(i+1));
    ui->output->insertPlainText("\n");

    ui->output->moveCursor(QTextCursor::End);
}
