#include "userregistration.h"
#include "ui_userregistration.h"
#include "QMessageBox"
#include <QDebug>
#include <QSqlQuery>
#include <db.h>
#include <QVector>
#include <QFileDialog>
#include <QFile>

#include <QRegularExpression>

UserRegistration::UserRegistration(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::UserRegistration)
{
    ui->setupUi(this);

}

UserRegistration::~UserRegistration()
{
    delete ui;
}


void UserRegistration::on_pbCheckUserName_clicked()
{

    QString enteredUserName = ui->leUserName->text();
    db *DB = new db;

    if (DB->isUserNameAvalible(enteredUserName)){
        ui->lUserNameStatus->setText("Avalible.");
        newUserName = enteredUserName;
    }else{
        ui->lUserNameStatus->setText("Error, Try other name.");
    }
}

QString UserRegistration::hashSha1(QString hashIt){
    QString hashedString = QString("%1").arg(QString(QCryptographicHash::hash(hashIt.toUtf8(),QCryptographicHash::Sha1).toHex()));
    return hashedString;
}

void UserRegistration::on_pbSelectFile_clicked()
{
    // get file name
    QFileDialog locateFile(this);
    locateFile.setFileMode(QFileDialog::AnyFile);

    QString LongFileName = locateFile.getOpenFileName();

    //passing the file location in LongFileName
    QFile f(LongFileName);
    QByteArray byteArrayHashed;
    if (f.open(QFile::ReadOnly)){
        QCryptographicHash hash(QCryptographicHash::Sha1);
        if (hash.addData(&f)){
            byteArrayHashed= hash.result();
        }
    }

    //hashedFile is a Data member of this class
    hashedFile = byteArrayHashed.toHex();
}


void UserRegistration::on_pbRegister_clicked()
{
    QVector<QString> userRegData;
    int numberOfPassSelected=0;

    //--------------------------------------------------password
    if (ui->cbPassword->checkState()==Qt::Checked){

        //appending "password" naem in userRegdata
        userRegData.append(ui->cbPassword->text());
        //password
        userRegData.append(hashSha1(ui->lePassword->text()));
        numberOfPassSelected++;
    }

    //--------------------------------------------------DOB
    if (ui->cbDob->checkState()==Qt::Checked){
        //check dataintegrity DOB. using regx


        userRegData.append(ui->cbDob->text());

        userRegData.append(hashSha1(ui->leDob->text()));

        numberOfPassSelected++;
    }

    //--------------------------------------------------PIN
    if (ui->cbPin->checkState()==Qt::Checked){
        //

        userRegData.append(ui->cbPin->text());

        userRegData.append(hashSha1(ui->lePin->text()));

        numberOfPassSelected++;
    }


    //-------------------------------------------------file
    if (ui->cbAnyFile->checkState()==Qt::Checked){
        if(hashedFile.length() != 0){
            userRegData.append("File");
            userRegData.append(hashedFile);

            numberOfPassSelected++;
        }else{
            qWarning()<<"ERROR!";
        }
    }

    //-------------------------------------------------email
    if (ui->cbEmail->checkState()==Qt::Checked){
        //
        //appending "password" in userRegdata
        userRegData.append(ui->cbEmail->text());
        //appending hashed pass
        userRegData.append(hashSha1(ui->leEmail->text()));

        numberOfPassSelected++;
    }

    //-------------------------------------------------Custom password
    if (ui->cbCustomPassword->checkState()==Qt::Checked){

        //appending "password" in userRegdata
        userRegData.append(ui->leCustomPassN->text());

        //appending hashed pass
        userRegData.append(hashSha1(ui->leCustomPass->text()));

        numberOfPassSelected++;
    }


    if(numberOfPassSelected == 3){
        //execute the sql statement to register the password.
        QSqlQuery query;
           query.prepare("INSERT INTO LOGIN (userName, passwordN1, password1, passwordN2, password2, passwordN3, password3) "
                         "VALUES (:userName, :passwordN1, :password1, :passwordN2, :password2, :passwordN3, :password3)");
           query.bindValue(0, newUserName);
           query.bindValue(1, userRegData[0]);
           query.bindValue(2, userRegData[1]);
           query.bindValue(3, userRegData[2]);
           query.bindValue(4, userRegData[3]);
           query.bindValue(5, userRegData[4]);
           query.bindValue(6, userRegData[5]);

           if(query.exec()){
               ui->label_2->setText("New User Registration Complete.");
           }else{
               ui->label_2->setText("Registration Failed");
           }


    }else{
        ui->label_2->setText("SELECT 3 PASSWORDS!");
    }


}



