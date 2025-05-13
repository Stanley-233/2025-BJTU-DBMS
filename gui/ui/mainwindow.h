#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QPlainTextEdit>
#include"Parser.h"
#include"CommandLine.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCreateTable();
    void onClearSql();
    void onLoadScript();
    void on_executeButton_clicked();
private:
    void updateDataView(const QString& data);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
