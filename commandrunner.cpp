#include "commandrunner.h"
#include "ui_commandrunner.h"

#include <functional>
#include <algorithm>
#include <memory>

#include <QDebug>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QStandardPaths>
#include <QStyle>
#include <QResource>
CommandRunner::CommandRunner(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandRunner)
{
    ui->setupUi(this);


    m_sysTray.setIcon(QIcon(":/root/icon.png"));
    auto contextMenu =  new QMenu(this);

    auto acShow = contextMenu->addAction("Show");
    connect(acShow, SIGNAL(triggered(bool)), this, SLOT(acShowClicked(bool)));

    auto acProp = contextMenu->addAction("Properties");
    connect(acProp, SIGNAL(triggered(bool)), this, SLOT(acPropClicked(bool)));

    auto acExit = contextMenu->addAction("Exit");
    connect(acExit, SIGNAL(triggered(bool)), this, SLOT(acExitClicked(bool)));


    connect(&m_sysTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    loadJson();

    loadSettings();

    m_sysTray.showMessage("ssa","as");
    m_sysTray.setContextMenu(contextMenu);
    m_sysTray.show();

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);

}

CommandRunner::~CommandRunner()
{
    delete ui;
}

bool CommandRunner::event(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            hide();
        }
    }
    return QMainWindow::event(event);
}

void CommandRunner::loadJson()
{

    QFile jsFile;
    // ~/.config/mx/CommandRunner/
    QString jsFilePath=QStandardPaths::locate(QStandardPaths::AppConfigLocation,"config.json");
    jsFile.setFileName(jsFilePath);
    qDebug() << "confg: "<<jsFilePath;

    QList<ConfigRow> configs;
    if (jsFile.open(QFile::ReadOnly))
    {
        auto bytes = jsFile.readAll();
        QJsonDocument doc= QJsonDocument::fromJson(bytes);
        auto procsJObj= doc.object()["processes"].toArray();
        for (const auto& procJObj: procsJObj)
        {
            ConfigRow row;
            row.name = procJObj.toObject()["name"].toString();
            row.program = procJObj.toObject()["path"].toString();
            row.interval = procJObj.toObject()["interval"].toInt();
            const auto& argsJObj= procJObj.toObject()["args"].toArray();

            for (const auto& argJobj: argsJObj)
            {
                row.args.append( argJobj.toString() );
                qDebug()<< argJobj.toString() ;
            }
            configs.append(row);
        }
    }

    m_configs = configs;
}

void CommandRunner::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    acShowClicked(true);
}

void CommandRunner::acPropClicked(bool trigger)
{

}

void CommandRunner::acExitClicked(bool trigger)
{
    QApplication::exit();
}

void CommandRunner::acShowClicked(bool trigger)
{
    show();
    raise();
    activateWindow();
}

void CommandRunner::timeout(int procId)
{

    if (m_procs[procId]->state() == QProcess::NotRunning)
    {
        m_procs[procId]->setArguments(m_configs[procId].args);
        m_procs[procId]->setProgram(m_configs[procId].program);
        m_procs[procId]->start();
    }
    else
    {
        print(procId, "Err: process busy\n");
    }

}

void CommandRunner::procReadyRead(int procId, int ch)
{
    //int i =
    auto bytes = m_procs[procId]->readAll();
    print(procId, QString(bytes));
}

void CommandRunner::print(int procId, QString text)
{
    QString outputLine = QDateTime::currentDateTime().toString("yy/mm/dd-HH:MM:ss");
    outputLine += " [";
    outputLine += m_configs[procId].name;
    outputLine += "] : ";
    outputLine += text;
    ui->teOutputDisplay->insertPlainText( outputLine );
}

void CommandRunner::loadSettings()
{

    for (auto& timer: m_timers)
    {
        timer->stop();
        delete timer;
    }

    m_timers.clear();

    for (auto& proc: m_procs)
    {
        proc->kill();
        proc->waitForFinished();
        delete proc;
    }


    m_procs.clear();
    for (int i=0;i<m_configs.size();i++)
    {
        qDebug() << m_configs[i].name <<" "<< m_configs[i].program << m_configs[i].args;
        auto ti = new QTimer();
        m_timers.append(ti);

        auto ap = new QProcess;
        m_procs.append(ap);

    }

    for (int i=0;i<m_procs.size();i++)
    {
        connect(m_procs[i], &QProcess::channelReadyRead, std::bind(&CommandRunner::procReadyRead, this, i, std::placeholders::_1));
    }
    for (int i=0;i<m_timers.size();i++)
    {
        connect(m_timers[i], &QTimer::timeout, std::bind(&CommandRunner::timeout, this, i));

        m_timers[i]->setInterval(m_configs[i].interval);
        m_timers[i]->start();
    }
    qDebug() <<"load";
}

void CommandRunner::saveSettings()
{
    qDebug() << "save";
}
