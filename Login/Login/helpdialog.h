#ifndef HELPDIALOG_H
#define HELPDIALOG_H
#include <QMainWindow>
#include <QMessageBox>//消息盒子

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
    ~HelpDialog();

private:
    Ui::HelpDialog *ui;
};
#endif // HELPDIALOG_H
