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

        // Write
        std::ostringstream os;
        os << avtomat_;
        std::string text = os.str();

        ui->textboxOutputFile->setText(QString::fromStdString(text));
        fi.close();
        fo.close();

        LoadGraph();
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

        // Write
        std::ostringstream os;
        os << avtomat;
        std::string text = os.str();

        ui->textboxOutputFile->setText(QString::fromStdString(text));

        LoadGraph();
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

void app::LoadGraph()
{
    // generate dot file content
    std::string dotContent = avtomat_.ToGraph();

    // Write to dot file
    std::ofstream fo("automaton.dot");
    fo << dotContent;
    fo.close();

    // Generate png from dot with GraphViz
    QProcess *dotProcess = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // qDebug() << env.toStringList();
    dotProcess->setProcessEnvironment(env);
    dotProcess->start("dot", QStringList() << "-Tpng" << "-oautomaton.png" << "automaton.dot");

    // Display the graph
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

void app::on_btnReadRegex_clicked()
{
    std::string regex = ui->txtboxInputRegex->text().toStdString();

    AbstractSyntaxTree ast(regex);
    qDebug() << QString::fromStdString(ast.toString());

    Automaton avtomat = ast.toNFA();

    std::ofstream fo("NFA.txt");
    std::string content = avtomat.ToFileContent("regex");
    qDebug() << QString::fromStdString(content);
    fo << content;
    fo.close();


    // generate dot file content
    std::string dotContent = avtomat.ToGraph();

    // Write to dot file
    std::ofstream fo1("NFA.dot");
    fo1 << dotContent;
    fo1.close();

    // Generate png from dot with GraphViz
    QProcess *dotProcess = new QProcess(this);
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    // qDebug() << env.toStringList();
    dotProcess->setProcessEnvironment(env);
    dotProcess->start("dot", QStringList() << "-Tpng" << "-oNFA.png" << "NFA.dot");
    dotProcess->waitForFinished();
    dotProcess->close();

    // Display the graph
    QString filename = "NFA.png";
    ui->lblGraph->setAlignment(Qt::AlignCenter);
    QPixmap pix;

    /** to check wether load ok */
    if(pix.load(filename)) {
        /** scale pixmap to fit in label'size and keep ratio of pixmap */
//        pix = pix.scaled(ui->lblGraph->size(),Qt::KeepAspectRatio);
        ui->lblGraph->setPixmap(pix);
    }

    /* check finite and list all words in language */
    std::vector<std::string> language;
    qDebug() << avtomat.ListAllWords(language);

    content = "";

    for(auto&& word: language) {
        content += word + "\n";
    }

    ui->textboxOutputFile->setText(QString::fromStdString(content));
}
