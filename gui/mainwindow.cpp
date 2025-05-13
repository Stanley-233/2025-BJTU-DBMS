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
    connect(ui->executeButton, &QPushButton::clicked, this, &MainWindow::on_executeButton_clicked);



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
    QString path = QFileDialog::getOpenFileName(this, "Open SQL Script", "", "SQL Files (*.dsql)");
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


void MainWindow::on_executeButton_clicked() {
    QString sqlText = ui->sqlEditor->toPlainText().trimmed();
    if (sqlText.isEmpty()) {
        QMessageBox::warning(this, "Warning", "SQL statement cannot be empty!");
        return;
    }

    CommandLine& cmd = CommandLine::getInstance();
    Parser& parser = cmd.getParser(); // 获取CommandLine中的Parser实例

    // 分割多条SQL语句，跳过空语句
    QStringList sqlStatements = sqlText.split(';', Qt::SkipEmptyParts);
    foreach (QString stmt, sqlStatements) {
        QString trimmedStmt = stmt.trimmed();
        if (trimmedStmt.isEmpty()) {
            continue;
        }
        trimmedStmt += ";"; // 为每个语句添加分号

        std::vector<std::string> tokens;
        int status = cmd.processSQL(trimmedStmt.toStdString(), tokens);

        if (status == -1) {
            QMessageBox::warning(this, "Error", "Missing semicolon in statement: " + trimmedStmt);
            continue; // 继续处理下一条语句
        }

        std::string resultStr = parser.parse(tokens); // 使用同一Parser实例

        // 处理结果显示逻辑
         if(!tokens.empty() && tokens[0] == "SELECT") {
             updateDataView(QString::fromStdString(resultStr));}

         else {
            QMessageBox::information(this, "Result", QString::fromStdString(resultStr));
        }
    }
}

// 处理Data View的函数
void MainWindow::updateDataView(const QString& data)
{
    if(data.startsWith("ERROR:")) {
        QMessageBox::warning(this, "Error", data);
        return;
    }

    QStringList rows = data.split("\n");
    if(rows.isEmpty()) return;

    // 获取列标题
    QStringList headers = rows[0].split(",");

    ui->dataTableWidget->clear();
    ui->dataTableWidget->setRowCount(rows.size() - 1);  // 减去标题行
    ui->dataTableWidget->setColumnCount(headers.size());
    ui->dataTableWidget->setHorizontalHeaderLabels(headers);

    // 填充数据
    for(int i = 1; i < rows.size(); i++) {
        QStringList cells = rows[i].split(",");
        for(int j = 0; j < cells.size() && j < headers.size(); j++) {
            QTableWidgetItem* item = new QTableWidgetItem(cells[j].trimmed());
            ui->dataTableWidget->setItem(i-1, j, item);
        }
    }

    ui->dataTableWidget->resizeColumnsToContents();
    ui->dataTableWidget->resizeRowsToContents();
}
MainWindow::~MainWindow() {
    delete ui;
}
