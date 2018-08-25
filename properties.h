#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class Properties;
}

class Properties : public QDialog
{
    Q_OBJECT

public:
    explicit Properties(QWidget *parent = 0);
    ~Properties();

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

    void on_addBtn_clicked();

    void on_remBtn_clicked();

signals:
    void loadSettings();
    void saveSettings();
private:
    Ui::Properties *ui;
};

#endif // PROPERTIES_H
