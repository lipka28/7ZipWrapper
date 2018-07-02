#ifndef MANAGE_H
#define MANAGE_H

#include <QObject>
#include <QStringList>
#include <QString>
#include <QFileDialog>
#include <QProcess>


class manage : public QObject
{
    Q_OBJECT
public:
    explicit manage(QObject *parent = nullptr);

    QString setOutput(QWidget *parent);
    QStringList addInputFiles(QWidget *parent);

    void clearAllFiles();
    int archiveEverything();

    QProcess *pt_zip_7 = &zip_7;



private:
    ~manage();

protected:
    QString output = NULL;
    QStringList inputFiles;
    QProcess zip_7;



signals:


public slots:
};

#endif // MANAGE_H
