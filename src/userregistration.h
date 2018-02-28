#ifndef USERREGISTRATION_H
#define USERREGISTRATION_H

#include <QWizardPage>

namespace Ui {
class UserRegistration;
}

class UserRegistration : public QWizardPage
{
    Q_OBJECT

public:
    explicit UserRegistration(QWidget *parent = 0);

    QString hashSha1(QString hashIt);




    ~UserRegistration();

private slots:
    void on_pbCheckUserName_clicked();

    void on_pbRegister_clicked();



    void on_pbSelectFile_clicked();

private:
    QString newUserName;
    QString hashedFile;
    Ui::UserRegistration *ui;
};

#endif // USERREGISTRATION_H
