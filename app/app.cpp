#include "app.h"
#include "ui_app.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QProcess>
#include <QTextCodec>

#include "../lib/AutomataLib/Automata/finitestateautomaton.h"
#include "../lib/AutomataLib/Automata/parser.h"
#include "../lib/AutomataLib/AbstractSyntaxTree/abstractsyntaxtree.h"

#include <iostream>
#include <fstream>
#include <errno.h>
#include <sstream>
#include <QDebug>
#include <QPixmap>

#include "Automata/nfatodfaconverter.h"

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
        Automaton* avtomat = parser.getNewAvtomat();

        // Validate test vector
        auto isDFA = parser.getTestIsDFA();
        auto isFinite = parser.getTestIsFinite();
        auto testWords = parser.getTestWords();

        avtomat->ValidateTestVector(isDFA, isFinite, testWords);

        // Write parsed content to file
        std::ofstream fo("ggout.txt");
        fo << *avtomat;

        fi.close();
        fo.close();

        LoadGraph(avtomat, "automaton", ui->lblGraph);
        EnumerateLanguage(avtomat);

        delete avtomat;

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
        Automaton* avtomat = parser.getNewAvtomat();

        // Validate test vector
        auto isDFA = parser.getTestIsDFA();
        auto isFinite = parser.getTestIsFinite();
        auto testWords = parser.getTestWords();

        avtomat->ValidateTestVector(isDFA, isFinite, testWords);

        // Write parsed content to file
        std::ofstream fo("parsed_content.txt");
        fo << *avtomat;

        fo.close();

        LoadGraph(avtomat, "automaton", ui->lblGraph);
        EnumerateLanguage(avtomat);

        delete avtomat;

    }  catch (const std::invalid_argument& ia) {
        QMessageBox::warning(this, "Error", ia.what());
    }
}

void app::LoadGraph(Automaton* avtomat, std::string fileName, QLabel* label)
{
    /* Output graphviz .dot file from automata */
    std::string dotContent = avtomat->toGraph();

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
    label->setAlignment(Qt::AlignCenter);
    QPixmap pix;

    /** to check wether load ok */
    if(pix.load(pngFileName)) {
        /** scale pixmap to fit in label'size and keep ratio of pixmap */
        // pix = pix.scaled(ui->lblGraph->size(),Qt::KeepAspectRatio);
        label->setPixmap(pix);
        label->setScaledContents(true);
    }
}

void app::EnumerateLanguage(Automaton* avtomat)
{
    std::vector<std::string> language;

    bool isLanguageFinite = avtomat->ListAllWords(language);
    if(!isLanguageFinite)
    {
        ui->textboxListLanguage->setText("The associated language is not finite DFA");
        return;
    }

    std::string content = "";

    for(auto&& word: language) {
        content += word + "\n";
    }

    ui->textboxListLanguage->setText(QString::fromStdString(content));
}

void app::on_btnReadRegex_clicked()
{
    /* Create NFA from regex */
    std::string regex = ui->txtboxInputRegex->text().toStdString();
    AbstractSyntaxTree ast(regex);
    FiniteStateAutomaton* fsa = new FiniteStateAutomaton(ast.toNFA());
//    this->avtomat_ = ast.toNFA();

    /* Output the NFA to a file */
    std::ofstream fo("NFA.txt");
    std::string content = fsa->toFileContent("regex");
    fo << content;
    fo.close();

    /* Show the NFA associated with the regex */
    LoadGraph(fsa, "NFA", ui->lblGraph);

//    /* Check if the associated language is finite, enumerate if it is */
    EnumerateLanguage(fsa);

    /* Generate DFA */
    NFAToDFAConverter converter(fsa);
    FiniteStateAutomaton* dfa = new FiniteStateAutomaton(converter.getDFA());
    LoadGraph(dfa, "DFA", ui->lblGraphDFA);

    delete fsa;
}
