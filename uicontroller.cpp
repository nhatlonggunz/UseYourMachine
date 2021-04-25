#include "uicontroller.h"
#include "ui_uicontroller.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include "Automata/automaton.h"
#include "Automata/parser.h"

#include <iostream>
#include <fstream>
#include <errno.h>
#include <sstream>


UIController::UIController(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UIController)
{
    ui->setupUi(this);
}

UIController::~UIController()
{
    delete ui;
}


void UIController::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    std::string name = fileName.toStdString();

    std::ifstream fi(name);
    Automaton avtomat = Parser::ReadFromStream(fi);

    std::ofstream fo("ggout.txt");
    fo << avtomat;

    std::ostringstream os;
    os << avtomat;
    std::string text = os.str();

    ui->textEdit->setText(QString::fromStdString(text));
    fi.close();
    fo.close();
}
