#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    setWindowTitle("Server, port: 8888");

    tcpServer = NULL;
    tcpSocket = NULL;

    tcpServer = new QTcpServer(this);//Create Server on TCP

    tcpServer->listen(QHostAddress::Any, 8888);//Listen on server using port 8888

    connect(tcpServer, &QTcpServer::newConnection,//when a connection between the server and client
            [=](){
                //when new connection arrived, create a socket to communicate with the client
                tcpSocket = tcpServer->nextPendingConnection();
                QString ip = tcpSocket->peerAddress().toString();//get the ip info of the client in socket
                quint16 port = tcpSocket->peerPort();//get the port info of the client in socket
                QString temp = QString("[%1:%2]:Connected Successfully").arg(ip).arg(port);

                ui->textEditRead->setText(temp);

                connect(tcpSocket, &QTcpSocket::readyRead,//when a connection told that the socket is ready
                        [=](){
                            QByteArray array = tcpSocket->readAll();//read all the info that is sent from client
                            ui->textEditRead->append(array);

                        }
                        );
            }
            );


}

ServerWidget::~ServerWidget()
{
    delete ui;
}


void ServerWidget::on_buttonSend_clicked()
{
    if (tcpSocket==NULL){
        return;
    }

    QString str = ui->textEditWrite->toPlainText();

    tcpSocket->write(str.toUtf8().data());//write to socket, as well as sending to the client

}

void ServerWidget::on_buttonClose_clicked()
{
    if (tcpSocket==NULL){
        return;
    }

    tcpSocket->disconnectFromHost();//disconnect the socket
    tcpSocket->close();//close the socket

    tcpSocket = NULL;
}
