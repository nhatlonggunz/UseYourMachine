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
    Parser parser = Parser();

    try {
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
        parser.ReadFromStream(fi);
        Automaton avtomat = parser.getAutomaton();

        // Validate test vector
        auto isDFA = parser.getTestIsDFA();
        auto isFinite = parser.getTestIsFinite();
        auto testWords = parser.getTestWords();

        avtomat.ValidateTestVector(isDFA, isFinite, testWords);

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
    }  catch (const std::invalid_argument& ia) {
        QMessageBox::warning(this, "Error", ia.what());
    }
}

void UIController::on_btnReadInputFile_clicked()
{
    Parser parser = Parser();

    try {
        // Read and write the whole file to input textbox
        std::string input = ui->textboxInputFile->toPlainText().toStdString();

        // Parse input
        std::istringstream ss(input);

        parser.ReadFromStream(ss);
        Automaton avtomat = parser.getAutomaton();

        // Write parsed content to file
        std::ofstream fo("ggout.txt");
        fo << avtomat;

        // Write
        std::ostringstream os;
        os << avtomat;
        std::string text = os.str();

        ui->textboxOutputFile->setText(QString::fromStdString(text));
    }  catch (const std::invalid_argument& ia) {
        QMessageBox::warning(this, "Error", ia.what());
    }
}
