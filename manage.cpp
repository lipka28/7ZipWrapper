#include "manage.h"

manage::manage(QObject *parent) : QObject(parent)
{

}

QString manage::setOutput(QWidget *parent)
{
    output = QFileDialog::getSaveFileName(parent,tr("Choose output"),(QString)"./untitled.7z",tr("7Zip File (*.7z)"));
    return output;
}

QStringList manage::addInputFiles(QWidget *parent)
{
    QStringList newlyAdded;
    newlyAdded = QFileDialog::getOpenFileNames(parent,tr("Slecet files to add to archive"),(QString)"./",tr("All (*.*)"));
    inputFiles = inputFiles + newlyAdded;
    inputFiles.removeDuplicates();
    return inputFiles;
}

void manage::clearAllFiles()
{
    inputFiles.clear();
}

int manage::archiveEverything()
{
    QString files = inputFiles.join(" ");
    if(inputFiles.count() == 0) return -1;
    else if (output == NULL) return 0;
    else{

#ifdef _WIN32
        switch (zip_7.state()) {
        case QProcess::NotRunning:
            zip_7.setWorkingDirectory(QCoreApplication::applicationDirPath());
            zip_7.start("7z.exe a "+output+" "+files);
            return 1;
            break;

        case QProcess::Running:
        case QProcess::Starting:
            return -3;
            break;
        default:
            return -2;
            break;
        }
#endif
#ifdef linux
        switch (zip_7.state()) {
        case QProcess::NotRunning:
            zip_7.start("7z a "+output+" "+files);
            return 1;
            break;

        case QProcess::Running:
        case QProcess::Starting:
            return -3;
            break;
        default:
            return -2;
            break;
        }
#endif

    }
}

manage::~manage()
{
    zip_7.terminate();
}
