// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../createtabledialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    refreshDatabaseTree();

    while(true) {
        // 创建并显示登录对话框
        QDialog loginDialog(this);
        loginDialog.setWindowTitle("登录");

        QFormLayout form(&loginDialog);

        QLineEdit usernameEdit;
        QLineEdit passwordEdit;
        passwordEdit.setEchoMode(QLineEdit::Password);

        form.addRow("用户名:", &usernameEdit);
        form.addRow("密码:", &passwordEdit);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                   Qt::Horizontal, &loginDialog);
        form.addRow(&buttonBox);

        std::string m_username, m_password;

        QObject::connect(&buttonBox, &QDialogButtonBox::accepted, [&]() {
            m_username = usernameEdit.text().toStdString();
            m_password = passwordEdit.text().toStdString();
            loginDialog.accept();
        });
        QObject::connect(&buttonBox, &QDialogButtonBox::rejected, [&]() {
            QMessageBox::warning(this, "警告", "未给出用户名，程序退出");
            QCoreApplication::quit(); // 如果取消登录则退出程序
        });
        bool flag = CommandLine::getInstance().Login(m_username, m_password);
        if (flag) break;
        QMessageBox::warning(this, "警告", "用户不存在或密码错误!");
    }

    // Connect signals
    connect(ui->actionCreateTable, &QAction::triggered, this, &MainWindow::onCreateTable);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::onClearSql);
    connect(ui->loadScriptButton, &QPushButton::clicked, this, &MainWindow::onLoadScript);
    connect(ui->databaseTreeWidget, &QTreeWidget::itemClicked,
            this, &MainWindow::onDatabaseItemClicked);
    connect(ui->databaseTreeWidget, &QTreeWidget::itemExpanded,
            [this](QTreeWidgetItem *item) {
                if (item->data(0, Qt::UserRole) == "database") {
                    loadTablesForDatabase(item, item->text(0));
                }
            });

}

void MainWindow::onDatabaseItemClicked(QTreeWidgetItem *item, int column) {
    // 处理点击事件的代码
    if (item->data(0, Qt::UserRole) == "table") {
        QString dbName = item->parent()->text(0);
        QString tableName = item->text(0);
        qDebug() << "Selected table:" << dbName << "." << tableName;
    }
}

void MainWindow::refreshDatabaseTree() {
    ui->databaseTreeWidget->clear();

    QDir systemDir("system");
    if (!systemDir.exists()) {
        systemDir.mkpath(".");
        return;
    }

    // 获取所有数据库（目录）
    QStringList databases = systemDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    foreach (const QString &dbName, databases) {
        QTreeWidgetItem *dbItem = new QTreeWidgetItem(ui->databaseTreeWidget);
        dbItem->setText(0, dbName);
        dbItem->setIcon(0, QIcon(":/icons/database.png"));
        dbItem->setData(0, Qt::UserRole, "database");

        // 延迟加载表结构
        dbItem->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    }
}

void MainWindow::loadTablesForDatabase(QTreeWidgetItem *dbItem, const QString& dbName) {
    dbItem->takeChildren();

    QDir dbDir(QString("system/%1").arg(dbName));
    QStringList tables;

    // 获取所有可能的表文件
    QStringList files = dbDir.entryList({"*.csv", "*.header"}, QDir::Files);
    QSet<QString> validTables;

    foreach (const QString &file, files) {
        QString baseName = QFileInfo(file).completeBaseName();
        // 检查是否同时存在两个文件
        if (dbDir.exists(baseName + ".csv") &&
            dbDir.exists(baseName + ".header")) {
            validTables.insert(baseName);
        }
    }

    foreach (const QString &table, validTables) {
        QTreeWidgetItem *tableItem = new QTreeWidgetItem(dbItem);
        tableItem->setText(0, table);
        tableItem->setIcon(0, QIcon(":/icons/table.png"));
        tableItem->setData(0, Qt::UserRole, "table");
    }
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
         refreshDatabaseTree();
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
