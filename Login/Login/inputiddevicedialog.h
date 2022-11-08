#ifndef INPUTIDDEVICEDIALOG_H
#define INPUTIDDEVICEDIALOG_H

#include <QDialog>
#include <string>
#include <QString>
#include <QTcpSocket>

namespace Ui {
class inputIdDeviceDialog;
}

class inputIdDeviceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit inputIdDeviceDialog(QWidget *parent = 0);
    ~inputIdDeviceDialog();

private slots:
    void readMessage();

    void on_btnCancel_clicked();

    void on_btnSure_clicked();

private:
    Ui::inputIdDeviceDialog *ui;
    QString inputId, message;
    bool haveRecvFlag = false;
};

#endif // INPUTIDDEVICEDIALOG_H
