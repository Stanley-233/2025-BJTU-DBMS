// createtabledialog.cpp
#include "createtabledialog.h"
#include <QPushButton>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>

CreateTableDialog::CreateTableDialog(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Table name input
    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(new QLabel("Table Name:"));
    tableNameEdit = new QLineEdit();
    nameLayout->addWidget(tableNameEdit);
    mainLayout->addLayout(nameLayout);

    // Fields area with scroll
    QScrollArea *scrollArea = new QScrollArea();
    QWidget *scrollWidget = new QWidget();
    fieldsLayout = new QVBoxLayout(scrollWidget);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    mainLayout->addWidget(scrollArea);

    // Add field button
    QPushButton *addButton = new QPushButton("Add Field");
    connect(addButton, &QPushButton::clicked, this, &CreateTableDialog::addFieldRow);
    mainLayout->addWidget(addButton);

    // Dialog buttons
    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttons);

    // Add initial field
    addFieldRow();
}

void CreateTableDialog::addFieldRow() {
    QWidget *fieldRow = new QWidget();
    QHBoxLayout *rowLayout = new QHBoxLayout(fieldRow);

    QLineEdit *nameEdit = new QLineEdit();
    QComboBox *typeCombo = new QComboBox();
    typeCombo->addItems({"INTEGER", "TEXT", "DECIMAL"});
    QPushButton *delBtn = new QPushButton("Delete");

    rowLayout->addWidget(nameEdit);
    rowLayout->addWidget(typeCombo);
    rowLayout->addWidget(delBtn);

    connect(delBtn, &QPushButton::clicked, [fieldRow](){
        fieldRow->deleteLater();
    });

    fieldsLayout->addWidget(fieldRow);
}

QString CreateTableDialog::getCreateTableSql() const {
    QString tableName = tableNameEdit->text().trimmed();
    if (tableName.isEmpty()) return "";

    QStringList columns;
    for (int i = 0; i < fieldsLayout->count(); ++i) {
        if (auto row = qobject_cast<QWidget*>(fieldsLayout->itemAt(i)->widget())) {
            QLineEdit *nameEdit = row->findChild<QLineEdit*>();
            QComboBox *typeCombo = row->findChild<QComboBox*>();

            QString name = nameEdit->text().trimmed();
            if (name.isEmpty()) continue;

            QString type = typeCombo->currentText();
            QString column = QString("%1 %2").arg(name, type);

            columns << column;
        }
    }

    if (columns.isEmpty()) return "";
    return QString("CREATE TABLE %1 \n    %2\n;").arg(tableName, columns.join(" \n    "));
}
