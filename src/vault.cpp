#include <QWizardPage>
#include <QDebug>
#include <QLockFile>
#include <QFile>
#include <windows.h>
#include "vault.h"
#include "username.h"



Vault::Vault(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::Vault)
{
    ui->setupUi(this);
    QSqlDatabase db1;
    qWarning()<<"Database constructor.";
    db1 = QSqlDatabase::addDatabase("QSQLITE");
    db1.setDatabaseName("C:/Locker/LockerDb.sqlite");
    if (db1.open()){
        qWarning()<< "Database: Connected.";
    }
    else{
        qWarning()<< "Database: Not connected.";
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT longFileName FROM FILE where useName == '"+USERNAME+"';");
    if (query->exec()){
        qWarning()<<"FILES LOADED FROM DB.";
    }else{
        qWarning()<<"FILE NOT LOADED FORM DB";
    }

    //--------------------------------------locked file modle
    QSqlQueryModel * lockedFileModel = new QSqlQueryModel();
    lockedFileModel->setQuery(*query);
    ui->listView->setModel(lockedFileModel);
    //-------------------------------list locked file query

    loadComboBox();

}

Vault::~Vault()
{
    delete ui;
}


void Vault::loadLockedFile(QString userN){
    QSqlDatabase db1;
    qWarning()<<"Database constructor.";
    db1 = QSqlDatabase::addDatabase("QSQLITE");
    db1.setDatabaseName("C:/Locker/LockerDb.sqlite");
    if (db1.open()){
        qWarning()<< "Database: Connected.";
    }
    else{
        qWarning()<< "Database: Not connected.";
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT longFileName FROM FILE where useName == '"+userN+"';");
    if (query->exec()){
        qWarning()<<"FILES LOADED FROM DB.";
    }else{
        qWarning()<<"FILE NOT LOADED FORM DB";
    }

    //--------------------------------------locked file modle
    QSqlQueryModel * lockedFileModel = new QSqlQueryModel();
    lockedFileModel->setQuery(*query);
    ui->listView->setModel(lockedFileModel);
    //-------------------------------list locked file query
    //on_comboBox_activated();

    //loading combobox
    loadComboBox();

}


void Vault::on_pbLockFile_clicked()
{

    //-----------------------------------------------SELECT FILE
    QFileDialog locateFile(this);
    locateFile.setFileMode(QFileDialog::AnyFile);
    fileName = locateFile.getOpenFileName();
    QString fileNameForDB = fileName;
    qWarning()<<"FILENAME: "<<fileName;

    //-----------------------------------------------FILE PATH CONVERSION FROM FS TO BS
    int fileLength = fileName.length();

    for (int i = 0; i < fileName.length(); i++){
        if (fileName[i] == QChar('/'))
            fileLength++;
    }
    for(int i = 0; i < fileLength; i++){
        if (fileName[i] == QChar('/')){
            //qWarning()<<" / found : ";
            fileName.insert((i+1),('\\'));
        }
    }//for loop

    fileName.remove(QChar('/'), Qt::CaseInsensitive);

    qWarning()<<"------------------------------"<<fileName;

    //======================MAGIC------
    QString qstr = fileName;
    qWarning()<<"+++++++++++++++"<<fileName;
    // Convert it into standard wstring.
    std::wstring str = qstr.toStdWString();
    // Get the wchar_t pointer to the standard string.
    const wchar_t *p = str.c_str ();

     //-----------------------------------------------LOCKING THE SELECTED FILE
    if (hFile = CreateFile(p, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL)){
        LockFile(hFile,0,0,1024,0);
        qWarning()<<"File is locked";
        //qWarning()<<vectorFileHandle.at(0);
        //qWarning()<< vectorFilePath.at(0);
        vectorFilePath.append(fileNameForDB);
        vectorFileHandle.append(hFile);
    }

    //-----------------------------------------------INSERT THE LOCKED FILE INTO DB
    DB.insertLockedFile(USERNAME,fileNameForDB);
    qWarning()<<"FILE INSERTED INTO FILE DB";


    /////gart

    loadLockedFile(USERNAME);
}//pblock file


void Vault::on_pbUnlockFile_clicked()
{
    //-----------------------------------------------SELECT FILE
    QString fileToUnlock = lockedFileFromCombobox;

    for(int i = 0; i < vectorFilePath.length() ; i++){
        if(fileToUnlock == vectorFilePath.at(i)){
            //HANDLE = vectorFileHandle.
            UnlockFile( vectorFileHandle.at(i),0,0,1024,0);
            CloseHandle( vectorFileHandle.at(i));

            vectorFileHandle.removeAt(i);
            vectorFilePath.removeAt(i);

            DB.removeLockedFile(fileToUnlock);
            loadLockedFile(USERNAME);
            break;
        }else{
            ui->lpathOfFile->setText("FILE IS NOT IN THE DB");
        }

    }
    loadComboBox();

}//PB unlock file


void Vault::on_comboBox_activated(const QString &arg1)
{
    qWarning()<<"arg1: "<<arg1;
    lockedFileFromCombobox = arg1;
}

void Vault::loadComboBox(){
    qWarning()<<"Loading combobox";
    QStringList list;
    for (int i = 0; i < vectorFilePath.length(); i++){
        list.append(vectorFilePath.at(i));
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(list);
}
