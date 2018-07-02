#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    core = new manage(this);
    ui->tableW_Input->setColumnCount(1);
    ui->tableW_Input->verticalHeader()->resizeSections(QHeaderView::Stretch);
    ui->tableW_Input->verticalHeader()->setVisible(false);
    ui->tableW_Input->setHorizontalHeaderLabels((QStringList)tr("File"));
    ui->lable_Status->setText(tr("Status: Waiting"));
    allDisabled(false);
    started = false;

    connect(core->pt_zip_7,&QProcess::stateChanged,this,&MainWindow::statusChanged);
    connect(core->pt_zip_7,&QProcess::readyReadStandardOutput,this,&MainWindow::readSTDOUT);
    connect(core->pt_zip_7,&QProcess::readyReadStandardError,this,&MainWindow::readSTDER);
    connect(core->pt_zip_7,SIGNAL(finished(int)),this,SLOT(justFinished(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::allDisabled(bool rule)
{
    rule = !rule;

    ui->pb_AddFile->setEnabled(rule);
    ui->pb_Browse->setEnabled(rule);
    ui->pb_Compress->setEnabled(rule);
    ui->pb_RemoveAll->setEnabled(rule);
    ui->tableW_Input->setEnabled(rule);
    ui->le_Output->setEnabled(rule);
}

void MainWindow::on_pb_Compress_clicked()
{
    started = false;
    int run = core->archiveEverything();
    if(run == -1)QMessageBox::warning(this,tr("Error 111"),tr("No Files to compress"));
    else if(run == 0)QMessageBox::warning(this,tr("Error 101"),tr("No Output File specified"));
    else if(run == -2)QMessageBox::warning(this,tr("Error 100"),tr("Something broke !"));
    else if(run == -3)QMessageBox::warning(this,tr("Error 010"),tr("Compression not yet finished"));
    else started = true;

}

void MainWindow::on_pb_RemoveAll_clicked()
{
    core->clearAllFiles();
    while(ui->tableW_Input->rowCount() > 0){

        for(int i = 0; i < ui->tableW_Input->rowCount(); i++)
        {
            ui->tableW_Input->cellWidget(i,0)->deleteLater();
            ui->tableW_Input->removeRow(i);
        }
    }

}

void MainWindow::on_pb_AddFile_clicked()
{
    QStringList files = core->addInputFiles(this);
    ui->tableW_Input->setRowCount(files.count());
    for(int i = 0; i < files.count(); i++)
    {
        ui->tableW_Input->setCellWidget(i,0,new QLabel(files.at(i)));
    }
    ui->tableW_Input->horizontalHeader()->resizeSections(QHeaderView::Stretch);
}

void MainWindow::on_pb_Browse_clicked()
{
    ui->le_Output->setText(core->setOutput(this));
}

void MainWindow::statusChanged(QProcess::ProcessState state)
{
    switch (state) {
    case QProcess::NotRunning:
        allDisabled(false);
        if(started == true)QMessageBox::about(this,tr("Success"),tr("Sucessfully Finished"));
        ui->lable_Status->setText(tr("Status: Waiting"));
        break;

    case QProcess::Running:
    case QProcess::Starting:
        ui->lable_Status->setText(tr("Status: Working..."));
        allDisabled(true);
        break;
    default:
        allDisabled(false);
        ui->lable_Status->setText(tr("Status: WHAT THE HELL JUST HAPPENED ???"));
        break;
    }
}

void MainWindow::readSTDOUT()
{
    std::cout << core->pt_zip_7->readAllStandardOutput().toStdString();
}

void MainWindow::readSTDER()
{
    std::cout << core->pt_zip_7->readAllStandardError().toStdString();
}

void MainWindow::justFinished(int)
{
    started = false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(started == true){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,tr("Are you sure?"),tr("7Zip is still working on something. Do you really want to quit?"),
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            core->pt_zip_7->terminate();
            event->accept();
        }else event->ignore();


    }else event->accept();

}


void MainWindow::on_pb_Close_clicked()
{
    if(started == true){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,tr("Are you sure?"),tr("7Zip is still working on something. Do you really want to quit?"),
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes){
            core->pt_zip_7->terminate();
            this->close();
        }


    }else this->close();
}
