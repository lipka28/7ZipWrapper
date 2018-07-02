#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <manage.h>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void allDisabled(bool rule);
    ~MainWindow();

private slots:
    void on_pb_Compress_clicked();

    void on_pb_RemoveAll_clicked();

    void on_pb_AddFile_clicked();

    void on_pb_Browse_clicked();

    void on_pb_Close_clicked();

    void statusChanged(QProcess::ProcessState state);

    void readSTDOUT();

    void readSTDER();

    void justFinished(int);

    virtual void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    manage * core;
    bool started;

public slots:



};

#endif // MAINWINDOW_H
