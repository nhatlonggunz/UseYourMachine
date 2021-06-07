#ifndef APP_H
#define APP_H

#include <QMainWindow>
#include "../lib/AutomataLib/Automata/automaton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class app; }
QT_END_NAMESPACE

class app : public QMainWindow
{
    Q_OBJECT

public:
    app(QWidget *parent = nullptr);
    ~app();

private slots:
    void on_actionOpen_triggered(); // Open file

    void on_btnReadInputFile_clicked();

    void on_btnGenerateGraph_clicked();

    void on_btnReadRegex_clicked();

private:
    Ui::app *ui;
    Automaton avtomat_;

    void LoadGraph(Automaton avtomat, std::string fileName);
    void EnumerateLanguage(Automaton avtomat);
};
#endif // APP_H
