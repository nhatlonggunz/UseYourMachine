#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class UIController; }
QT_END_NAMESPACE

class UIController : public QMainWindow
{
    Q_OBJECT

public:
    UIController(QWidget *parent = nullptr);
    ~UIController();

private slots:
    void on_actionOpen_triggered(); // Open file

    void on_btnReadInputFile_clicked();

private:
    Ui::UIController *ui;
};
#endif // UICONTROLLER_H
