#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QHostAddress>

ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);

    tcpSocket = NULL;

    tcpSocket = new QTcpSocket(this);//create a socket

    connect(tcpSocket, &QTcpSocket::connected,//when a socket connect
            [=](){
                ui->textEditRead->setText("Connected with server successfully");

            }
            );

    connect(tcpSocket, &QTcpSocket:: readyRead,//socket is ready
            [=](){
                QByteArray array = tcpSocket->readAll();//read the socket
                ui->textEditRead->append(array);
            }
            );
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_buttonConnect_clicked()
{
    QString ip = ui->lineEditIP->text();//get ip
    qint16 port = ui->lineEditPort->text().toInt();//get port
    tcpSocket->connectToHost(QHostAddress(ip),port);//connect to the specific ip and port
}

void ClientWidget::on_buttonSend_clicked()
{
    QString str = ui->textEditWrite->toPlainText();
    tcpSocket->write(str.toUtf8().data());//write to socket
}

void ClientWidget::on_buttonClose_clicked()
{
    tcpSocket->disconnectFromHost();//socket disconnect
    tcpSocket->close();//socket close
    tcpSocket = NULL;
}
