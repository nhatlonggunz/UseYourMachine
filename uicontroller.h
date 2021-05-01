#ifndef UICONTROLLER_H
#define UICONTROLLER_H

#include <QMainWindow>
#include "Automata/automaton.h"

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

    void on_btnGenerateGraph_clicked();

    void on_btnReadRegex_clicked();

private:
    Ui::UIController *ui;
    Automaton avtomat_;

    void LoadGraph();
};
#endif // UICONTROLLER_H
