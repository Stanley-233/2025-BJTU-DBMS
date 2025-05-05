// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../createtabledialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Connect signals
    connect(ui->actionCreateTable, &QAction::triggered, this, &MainWindow::onCreateTable);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearSql);
    connect(ui->loadScriptButton, &QPushButton::clicked, this, &MainWindow::onLoadScript);
}

void MainWindow::onCreateTable() {
    CreateTableDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QString sql = dlg.getCreateTableSql();
        if (!sql.isEmpty()) {
            ui->sqlEditor->setPlainText(sql);
        }
    }
}

void MainWindow::onClearSql() {
    ui->sqlEditor->clear();
}

void MainWindow::onLoadScript() {
    QString path = QFileDialog::getOpenFileName(this, "Open SQL Script", "", "SQL Files (*.sql)");
    if (path.isEmpty()) return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    ui->sqlEditor->setPlainText(in.readAll());
    file.close();
}

MainWindow::~MainWindow() {
    delete ui;
}
