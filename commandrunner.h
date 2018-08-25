#ifndef COMMANDRUNNER_H
#define COMMANDRUNNER_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QProcess>
#include <QTimer>
#include <QProcess>
#include <QSettings>

#include "properties.h"

namespace Ui {
class CommandRunner;
}

struct ConfigRow
{
    QString name;
    QString program;
    QStringList args;
    int interval;
};

class CommandRunner : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandRunner(QWidget *parent = 0);
    ~CommandRunner();
    bool event(QEvent *event);
private:

    void loadJson();

    Ui::CommandRunner *ui;
    QSystemTrayIcon m_sysTray;
    QList<QTimer*> m_timers;
    QList<QProcess*> m_procs;
    QSettings m_settings;

    QList<ConfigRow> m_configs;
public slots:
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void acPropClicked(bool trigger);
    void acExitClicked(bool trigger);
    void acShowClicked(bool trigger);

    void timeout(int procId);

    void procReadyRead(int procId, int ch);

    void print(int procId, QString text);

    void loadSettings();
    void saveSettings();
};

#endif // COMMANDRUNNER_H
