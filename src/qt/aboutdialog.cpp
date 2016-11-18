#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "clientmodel.h"

#include "version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
}

void AboutDialog::setModel(ClientModel *model)
{
    if(model)
    {
        ui->versionLabel->setText(model->formatFullVersion());
        
        QString s = ui->copyrightLabel->text();
        s.replace(QString("[CLIENT_LAST_COPYRIGHT]"),
            QString::fromStdString(CLIENT_LAST_COPYRIGHT));
        ui->copyrightLabel->setText(s);
    }
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_buttonBox_accepted()
{
    close();
}
