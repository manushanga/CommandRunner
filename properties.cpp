#include "properties.h"
#include "ui_properties.h"

#include <QDebug>
#include <QTableWidgetItem>

Properties::Properties(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Properties)
{

    ui->setupUi(this);
    //ui->buttonBox->addButton(new QAbstractButton(parent);
}

Properties::~Properties()
{
    delete ui;
}


void Properties::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok)
    {
        hide();
        emit loadSettings();
        emit saveSettings();
    }
    else if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Cancel)
    {
        hide();
    }

}

void Properties::on_addBtn_clicked()
{
    auto mdl=ui->tableWidget->model();
    mdl->insertRow(0);
    mdl->setData(mdl->index(0,0),QString(""));
    mdl->setData(mdl->index(0,1),QString(""));
    mdl->setData(mdl->index(0,2),int(0));

    //ui->tableWidget->itemAt(0,0)->setText("22");
    //ui->tableWidget->itemAt(0,1)->setText("s22");
}


void Properties::on_remBtn_clicked()
{

}
