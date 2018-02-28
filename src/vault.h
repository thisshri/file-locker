#ifndef VAULT_H
#define VAULT_H

#include <QWizardPage>
#include <QDebug>
#include <QLockFile>
#include <QFile>
#include <windows.h>
#include "ui_vault.h"

#include <windows.h>

#include <QFile>
#include <QLockFile>
#include <QString>
#include <QFileOpenEvent>
#include <QFileDialog>
#include <QDir>
#include <QDirModel>
#include <QtMessageHandler>
#include <QtDebug>
#include <sys/file.h>

#include <username.h>

#include <unistd.h>
#include <cstdlib>

#include "db.h"
#include "pathandhandle.h"

namespace Ui {
class Vault;
}

class Vault : public QWizardPage
{
    Q_OBJECT

public:
    explicit Vault(QWidget *parent = 0);
    void loadLockedFile(QString userN);

    void loadComboBox();

    void lockFile(HANDLE hFileIn);
    void unLookFile(HANDLE hFileIn);
    HANDLE locateFileWithHandle();
    ~Vault();

private slots:
    void on_pbLockFile_clicked();
    void on_pbUnlockFile_clicked();

    void on_comboBox_activated(const QString &arg1);

private:

    //QSqlDatabase DB;
    db DB;
    QString fileName;
    //QFile file;
    Ui::Vault *ui;
    //LPCWSTR wStr; // = L"C:\\Users\\shri\\Desktop\\test folder\\00.txt"
    HANDLE hFile;
    QString lockedFileFromCombobox;

};

#endif // VAULT_H
