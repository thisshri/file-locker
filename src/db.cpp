#include "db.h"
#include <QDebug>

db::db()
{
    qWarning()<<"Database constructor.";
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("C:/Locker/LockerDb.sqlite");
    if (DB.open()){
        qWarning()<< "Database: Connected.";
    }
    else{
        qWarning()<< "Database: Not connected.";
    }
}//Constructor


QString db::getPassN1(){
    return db::passwordN1;
}

QString db::getPassN2(){
    return db::passwordN2;
}

QString db::getPassN3(){
    return db::passwordN3;
}

QString db::getPass1(){
    return db::password1;
}

QString db::getPass2(){
    return db::password2;
}

QString db::getPass3(){
    return db::password3;
}

QString db::getUserN(){
    return db::userName;
}


//bool fileLockByUser(QString userN){
//
//}

QVector<QString> db::getAllLockedFile(){
    QSqlQuery query;
    query.prepare("select longFileName from file");
    //int totalLockedFile = query.record().indexOf("'longFileName'");
    query.exec();
    QVector<QString> result;

    //int i = 0;
    while(query.next()){
       result.append(query.value(0).toString());
        qWarning()<<"FILE:";

    }
    qWarning()<<"NUMBER OF FILE APPEDNED TO VECTOR:"<<result.length();
    return result;

}


QString db::getLoginInfo(QString inUserName){

    QSqlQuery query;
    query.exec("SELECT passwordN1, passwordN2, passwordN3, password1, password2, password3, userName FROM LOGIN WHERE userName == '"+inUserName+"'");

    while (query.next()){
        passwordN1 = query.value(0).toString();
        passwordN2 = query.value(1).toString();
        passwordN3 = query.value(2).toString();
        password1 = query.value(3).toString();
        password2 = query.value(4).toString();
        password3 = query.value(5).toString();
        userName = query.value(6).toString();

    }
        qWarning()<<"the user name in database: "<<userName;

    return userName;
}

bool db::isUserNameAvalible(QString inUserName){
    bool createNewUser = 0;

    QSqlQuery query;
    query.exec("SELECT userName FROM LOGIN WHERE userName == '"+inUserName+"'");


    QString userNameFromDb;
    while (query.next()){
        userNameFromDb = query.value(0).toString();
    }

    qDebug() << query.executedQuery();
    qDebug() << query.lastError();
    qWarning()<<"entered username: "<<inUserName;
    qWarning()<<"output form db: "<<userNameFromDb;

    if(inUserName == userNameFromDb){
        qWarning()<<"username already taken";
        return 0;
    }else{
        qWarning()<<"you can use this name";
        return 1;
    }

    return createNewUser;

}


bool db::insertLockedFile(QString useName, QString lFileName){
    QSqlQuery query;
        query.prepare("INSERT INTO FILE (useName, longFileName) "
                      "VALUES (:useName, :longFileName)");
        query.bindValue(0, useName);
        query.bindValue(1, lFileName);
        //query.bindValue(2, "Simpson");
        return query.exec();
}
bool db::removeLockedFile(QString lockedFile){
    QSqlQuery query("DELETE FROM FILE WHERE longFileName = '"+lockedFile+"'");
    /*
    query.prepare("INSERT INTO person (id, forename, surname) "
                      "VALUES (:id, :forename, :surname)");
        query.bindValue(0, 1001);
        query.bindValue(1, "Bart");
        query.bindValue(2, "Simpson");
        */
    return query.exec();
}


QString db::checkLockedFile(QString leUnlockFile){


    QSqlQuery query ;
    query.prepare("select longFileName from file where longFileName == '"+leUnlockFile+"'");
    query.exec();
    QString result;
    while(query.next()){
        result = query.value(0).toString();
    }


    return result;

}

