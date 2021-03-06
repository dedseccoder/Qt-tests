#include "singinwindow.h"
#include "ui_singinwindow.h"
#include "Repository.h"
#include "API.h"

#include <QtNetwork>
#include <QtDebug>

SingInWindow::SingInWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingInWindow)
{
    ui->setupUi(this);
    ui->PasswordInput->setText("");
    ui->UserNameInput->setText("");
}

SingInWindow::~SingInWindow()
{
    delete ui;
}

void SingInWindow::on_Sing_In_clicked()
{
    //Repository* repository = Repository::getInstance();
    //API *api = (API *)repository->getItem("API");

    // TODO: USE api object for requests
    // Example:
    // api->Auth();

    //link for the app to server
    QNetworkRequest request (QUrl("http://213.59.157.203/MusicXBackend/api/authenticate"));
    //QNetworkRequest request (QUrl("http://213.59.157.203/api/authenticate"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkAccessManager man;

    //work with input
    QString username_input, password_input;

    username_input = ui->UserNameInput->text();
    password_input = ui->PasswordInput->text();
    //create JSON
    QJsonObject JSON;
    JSON.insert("username", username_input);
    JSON.insert("password", password_input);
    //send JSON
    QNetworkReply *reply = man.post(request, QJsonDocument(JSON).toJson());
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    if (reply->error())
    {
        qDebug () << "ERROR: " << reply->error() << endl;
    }
    //convert reply to JSON (i hope)
    QJsonDocument reply_document = QJsonDocument::fromJson(reply->readAll());
    //get code response HTTP
    QVariant response_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    //console output
    qDebug () << "start reading sing in response" << endl;
    qDebug () << "response code: " << response_code << endl;
    //getting our token from JSON reply
    QString token = reply_document.object().value("token").toString();
    qDebug () << "Token = " << token << endl;
    qDebug () << "end of reading sing in response" << endl;

    reply->deleteLater();
    this->close();

}
