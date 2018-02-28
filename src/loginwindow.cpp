#include <QApplication>
#include "loginwindow.h"
#include "ui_loginwindow.h"

#include "userregistration.h"
#include "ui_userregistration.h"

#include "vault.h"

#include <QFileDialog>
#include <QFile>
#include <username.h>
#include <QSystemTrayIcon>
#include <QRegularExpression>


QVector<QString>vectorFilePath;
QVector<HANDLE>vectorFileHandle;

QString USERNAME;

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->pbLocateFile1->setHidden(1);
    ui->pbLocateFile2->setHidden(1);
    ui->pbLocateFile3->setHidden(1);
    ui->lePassword1->setEchoMode(QLineEdit::Password);
    ui->lePassword2->setEchoMode(QLineEdit::Password);
    ui->lePassword3->setEchoMode(QLineEdit::Password);


    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->setIcon(QIcon("C://locker//lockimg.png"));
    trayIcon->setToolTip("Right click to open the Locker");
    trayIcon->show();

    ui->pbLogin->setDisabled(1);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == ( QSystemTrayIcon::DoubleClick )){
        if (this->isHidden() || this->isMinimized() ){
            //this->activateWindow();QSystemTrayIcon::Trigger ||
            this->show();

            qWarning()<<this->isHidden();
            qWarning()<<this->isMinimized();

            qWarning()<<"SHOWING WINDOW";
        }else{
            qWarning()<<"Already visible";
        }
    }
}


QString LoginWindow::HashFile(){

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
    return byteArrayHashed.toHex();
}

void LoginWindow::on_pbRegister_clicked()
{
    UserRegistration* uReg = new UserRegistration();
    uReg->show();
    uReg->setWindowTitle("User Registration.");
    uReg->setWindowIcon(QIcon("C:\\locker\\lockimg.png"));

}


void LoginWindow::on_pbUserName_clicked()
{
    //USERNAME;
    //getting the user name from the line edit.
    QString leUserName = ui->leUserName->text();

    if (leUserName.length() >= 4){
        db *DataBase = new db();

        if (leUserName == DataBase->getLoginInfo(leUserName)){

            ui->pbLogin->setDisabled(0);

            validUserName = DataBase->getLoginInfo(leUserName);
            ui->lProceed->setText("Enter the following passwords.");

            //init USERNAME
            USERNAME = leUserName;
            qWarning()<<"USERNAME TO BE USERD IN VAULT: "<<USERNAME;


            if (DataBase->getPassN1() != "File"){
                ui->pbLocateFile1->setHidden(1);
                ui->lePassword1->setHidden(0);
            }else{
                ui->pbLocateFile1->setHidden(0);
                ui->lePassword1->setHidden(1);
            }
            if (DataBase->getPassN2() != "File"){
                ui->pbLocateFile2->setHidden(1);
                ui->lePassword2->setHidden(0);
            }else{
                ui->pbLocateFile2->setHidden(0);
                ui->lePassword2->setHidden(1);
            }
            if (DataBase->getPassN3() != "File"){
                ui->pbLocateFile3->setHidden(1);
                ui->lePassword3->setHidden(0);
            }else{
                ui->pbLocateFile3->setHidden(0);
                ui->lePassword3->setHidden(1);
            }

            ui->lPassword1->setText(DataBase->getPassN1());
            ui->lPassword2->setText(DataBase->getPassN2());
            ui->lPassword3->setText(DataBase->getPassN3());
        }else{
            ui->lProceed->setText("Enter Valid Username!");

        }

        delete DataBase;
    }
    else{
        ui->lUserName->setText("Username: ERROR");
    }

    qWarning()<<"the user name is "<<validUserName;
    //USERNAME = validUserName;

}//Proceed button


void LoginWindow::on_pbLogin_clicked()
{
    ui->lProceed->clear();

    ui->pbLogin->setDisabled(0);

    db *DB = new db;
    DB->getLoginInfo(validUserName);


    //ui->pbLocateFile->setDisabled();


    //login verification
    QString pass1 = ui->lePassword1->text();
    QString pass2 = ui->lePassword2->text();
    QString pass3 = ui->lePassword3->text();
    QString hashedPass1, hashedPass2, hashedPass3;

    if (DB->getPassN1() =="File"){
        hashedPass1 = hashedFileFromLogin;

    }else{
        hashedPass1 = QString("%1").arg(QString(QCryptographicHash::hash(pass1.toUtf8(),QCryptographicHash::Sha1).toHex()));
    }
    if (DB->getPassN2() =="File"){
        hashedPass2 = hashedFileFromLogin;
    }else{
        hashedPass2 = QString("%1").arg(QString(QCryptographicHash::hash(pass2.toUtf8(),QCryptographicHash::Sha1).toHex()));
    }
    if (DB->getPassN3() =="File"){
        hashedPass3 = hashedFileFromLogin;
    }else{
        hashedPass3 = QString("%1").arg(QString(QCryptographicHash::hash(pass3.toUtf8(),QCryptographicHash::Sha1).toHex()));
    }




    if ((DB->getPass1() == hashedPass1) && (DB->getPass2() == hashedPass2) && (DB->getPass3() == hashedPass3)){
        qWarning()<<"USERNAME, VALID USERNAME"<<USERNAME<<","<<validUserName;
        USERNAME = validUserName;
        Vault* vault = new Vault();
        vault->show();
        qWarning()<<"USERNAME:"<<USERNAME<<","<<validUserName;
        //Vault vault;
       // vault.show();

    }else{
        ui->lProceed->setText("Enter correct password.");

    }
    qWarning()<<"login button clicked.";
    delete DB;
    ui->lePassword1->clear();
    ui->lePassword2->clear();
    ui->lePassword3->clear();

}


void LoginWindow::on_pbLocateFile1_clicked()
{
    hashedFileFromLogin = HashFile();
}

void LoginWindow::on_pbLocateFile2_clicked()
{
    hashedFileFromLogin = HashFile();
}

void LoginWindow::on_pbLocateFile3_clicked()
{
    hashedFileFromLogin = HashFile();
}



HANDLE initLockFileHandle(QString fileFromDb)
{
    //-----------------------------------------------FILE PATH CONVERSION FROM FS TO BS
    int fileLength = fileFromDb.length();

    for (int i = 0; i < fileFromDb.length(); i++){
        if (fileFromDb[i] == QChar('/'))
            fileLength++;
    }
    for(int i = 0; i < fileLength; i++){
        if (fileFromDb[i] == QChar('/')){
            //qWarning()<<" / found : ";
            fileFromDb.insert((i+1),('\\'));
        }
    }//for loop

    fileFromDb.remove(QChar('/'), Qt::CaseInsensitive);

    //qWarning()<<"------------------------------"<<fileFromDb;

    //======================MAGIC------
    QString qstr = fileFromDb;
    qWarning()<<"+++++++++++++++"<<fileFromDb;
    // Convert it into standard wstring.
    std::wstring str = qstr.toStdWString();
    // Get the wchar_t pointer to the standard string.
    const wchar_t *p = str.c_str ();

     //-----------------------------------------------LOCKING THE SELECTED FILE
    HANDLE iHfile;
    if (iHfile = CreateFile(p, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)){
        LockFile(iHfile,0,0,1024,0);
        //qWarning()<<"File is locked";
        //qWarning()<<vectorFileHandle.at(0);
        //qWarning()<< vectorFilePath.at(0);
        //vectorFilePath.append(fileNameForDB);
        //vectorFileHandle.append(hFile);

    }
    return iHfile;

}//pblock file


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setWindowIcon();

    LoginWindow w;
    a.setQuitOnLastWindowClosed(0);

    //init file lock
    db DB;
    QVector<QString> allFilesFromDB = DB.getAllLockedFile();
    if(allFilesFromDB.length() > 0){
        for (int i = 0; i < allFilesFromDB.length(); i++){
            qWarning()<<"]]]]]]]]]]]"<<allFilesFromDB.at(i);

            vectorFileHandle.append(initLockFileHandle(allFilesFromDB.at(i)));
            vectorFilePath.append(allFilesFromDB.at(i));
        }
    }

    w.setWindowIcon(QIcon("C:\\locker\\lockimg.png"));
    w.show();


    return a.exec();
}

