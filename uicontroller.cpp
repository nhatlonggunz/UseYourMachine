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
    // User choose file
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    std::string name = fileName.toStdString();

    // Read and write the whole file to input textbox
    std::ifstream fi(name);
    std::stringstream buffer;
    buffer << fi.rdbuf();
    ui->textboxInputFile->setText(QString::fromStdString(buffer.str()));

    // Parse input
    fi.seekg(0);
    Automaton avtomat = Parser::ReadFromStream(fi);

    // Write parsed content to file
    std::ofstream fo("ggout.txt");
    fo << avtomat;

    // Write
    std::ostringstream os;
    os << avtomat;
    std::string text = os.str();

    ui->textboxOutputFile->setText(QString::fromStdString(text));
    fi.close();
    fo.close();
}
