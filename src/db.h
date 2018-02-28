#ifndef DB_H
#define DB_H

#include <QtSql>
#include <QSqlQuery>

#include <QDebug>
#include <QString>
#include <QVector>


class db
{

    QSqlDatabase DB;

    QString passwordN1;
    QString passwordN2;
    QString passwordN3;

    QString password1;
    QString password2;
    QString password3;

    QString userName;
    //QString newUserName;

public:
    db();

    QString getLoginInfo(QString inUserName);
    bool isUserNameAvalible(QString inUserName);
    bool insertLockedFile(QString useName, QString lFileName);
    bool removeLockedFile(QString lockedFile);
    QString checkLockedFile(QString leUnlockFile);
    QVector<QString> getAllLockedFile();

    QString getPassN1();
    QString getPassN2();
    QString getPassN3();
    QString getPass1();
    QString getPass2();
    QString getPass3();
    QString getUserN();

};

#endif // DB_H
