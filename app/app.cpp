#include "app.h"
#include "ui_app.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QProcess>
#include <QTextCodec>

#include "../lib/AutomataLib/Automata/automaton.h"
#include "../lib/AutomataLib/Automata/parser.h"
#include "../lib/AutomataLib/AbstractSyntaxTree/abstractsyntaxtree.h"

#include <iostream>
#include <fstream>
#include <errno.h>
#include <sstream>
#include <QDebug>


app::app(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::app)
{
    ui->setupUi(this);
}

app::~app()
{
    delete ui;
}

void app::on_actionOpen_triggered()
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
        avtomat_ = parser.getAutomaton();

        // Validate test vector
        auto isDFA = parser.getTestIsDFA();
        auto isFinite = parser.getTestIsFinite();
        auto testWords = parser.getTestWords();

        avtomat_.ValidateTestVector(isDFA, isFinite, testWords);

        // Write parsed content to file
        std::ofstream fo("ggout.txt");
        fo << avtomat_;

        // Write to UI
        std::ostringstream os;
        os << avtomat_;
        std::string text = os.str();

        ui->textboxOutputFile->setText(QString::fromStdString(text));
        fi.close();
        fo.close();

        LoadGraph(this->avtomat_, "automaton");
    }  catch (const std::invalid_argument& ia) {
        QMessageBox::warning(this, "Error", ia.what());
    }
}

void app::on_btnReadInputFile_clicked()
{
    Parser parser = Parser();

    try {
        // Read and write the whole file to input textbox
        std::string input = ui->textboxInputFile->toPlainText().toStdString();

        // Parse input
        std::istringstream ss(input);

        parser.ReadFromStream(ss);
        Automaton avtomat = parser.getAutomaton();
        this->avtomat_ = avtomat;

        // Write parsed content to file
        std::ofstream fo("ggout.txt");
        fo << avtomat;

        // Write to UI textbox
        std::ostringstream os;
        os << avtomat;
        std::string text = os.str();

        ui->textboxOutputFile->setText(QString::fromStdString(text));

        LoadGraph(this->avtomat_, "automaton");
    }  catch (const std::invalid_argument& ia) {
        QMessageBox::warning(this, "Error", ia.what());
    }
}

void app::on_btnGenerateGraph_clicked()
{
    std::string dotContent = avtomat_.ToGraph();

    std::ofstream fo("automaton.dot");
    fo << dotContent;
    fo.close();

//    std::string cmd = "dot -Tpng -oautomaton.png automaton.dot";
//    system(cmd.c_str());
    QProcess *dotProcess = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    qDebug() << env.toStringList();
    dotProcess->setProcessEnvironment(env);
    dotProcess->start("dot", QStringList() << "-Tpng" << "-oautomaton.png" << "automaton.dot");
//    dotProcess->start("C:\\Long\\Fontys\\ALE2\\Graphviz\\bin\\dot.exe", QStringList() << "-v");



    QString filename = "automaton.png";
    ui->lblGraph->setAlignment(Qt::AlignCenter);
    QPixmap pix;

    /** to check wether load ok */
    if(pix.load(filename)) {
        /** scale pixmap to fit in label'size and keep ratio of pixmap */
//        pix = pix.scaled(ui->lblGraph->size(),Qt::KeepAspectRatio);
        ui->lblGraph->setPixmap(pix);
    }
}

void app::LoadGraph(Automaton avtomat, std::string fileName)
{
    /* Output graphviz .dot file from automata */
    std::string dotContent = avtomat.ToGraph();

    std::ofstream fo(fileName + ".dot");
    fo << dotContent;
    fo.close();

    /* Generate png from dot with GraphViz */
    QProcess *dotProcess = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // qDebug() << env.toStringList();

    dotProcess->setProcessEnvironment(env);
    dotProcess->start("dot",
                      QStringList() << "-Tpng"
                                    << ("-o" + fileName + ".png").c_str()
                                    << (fileName + ".dot").c_str());
    dotProcess->waitForFinished();
    dotProcess->close();

    /* Display the graph */
    QString pngFileName = (fileName + ".png").c_str();
    ui->lblGraph->setAlignment(Qt::AlignCenter);
    QPixmap pix;

    /** to check wether load ok */
    if(pix.load(pngFileName)) {
        /** scale pixmap to fit in label'size and keep ratio of pixmap */
//        pix = pix.scaled(ui->lblGraph->size(),Qt::KeepAspectRatio);
        ui->lblGraph->setPixmap(pix);
    }
}

void app::on_btnReadRegex_clicked()
{
    /* Create NFA from regex */
    std::string regex = ui->txtboxInputRegex->text().toStdString();
    AbstractSyntaxTree ast(regex);
    Automaton avtomat = ast.toNFA();

    /* Output the NFA to a file */
    std::ofstream fo("NFA.txt");
    std::string content = avtomat.ToFileContent("regex");
    fo << content;
    fo.close();

    /* Show the NFA associated with the regex */
    LoadGraph(avtomat, "NFA");

    /* Check if the associated language is finite, enumerate if it is */
    std::vector<std::string> language;
    content = "";

    for(auto&& word: language) {
        content += word + "\n";
    }

    ui->textboxOutputFile->setText(QString::fromStdString(content));
}
