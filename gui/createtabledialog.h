
#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QVBoxLayout>

class CreateTableDialog : public QDialog {
    Q_OBJECT
public:
    explicit CreateTableDialog(QWidget *parent = nullptr);
    QString getCreateTableSql() const;

private slots:
    void addFieldRow();

private:
    QLineEdit *tableNameEdit;
    QVBoxLayout *fieldsLayout;
};
