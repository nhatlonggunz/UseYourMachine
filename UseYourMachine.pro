TEMPLATE = subdirs

SUBDIRS = \
    lib/AutomataLib \
    TestYourMachine \
    app

CONFIG += ordered

### copy test input files to build folder
copydata.commands = $(COPY_DIR) $$shell_path($$PWD/TestYourMachine/InputFiles) $$shell_path($$OUT_PWD/Example_input_files)
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
###
