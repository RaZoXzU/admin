#include "mainmenugui.h"
#include "ui_mainmenugui.h"

MainMenuGUI::MainMenuGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenuGUI)
{
    ui->setupUi(this);
    connector = new SQL();

    ui->l_welcome->setText("Witaj "+connector->welcomeFunc()+" !");
    //ui->page_3->hide();

    int userLvl = connector->getLvl();
    if(userLvl==9)
    {
        ui->page_4->setEnabled(true);
    }
    else
    {
        ui->page_4->setEnabled(false);
    }

}

MainMenuGUI::~MainMenuGUI()
{
    delete ui;
}

void MainMenuGUI::on_pushButton_clicked()
{
    newCar *form = new newCar;
    form->show();
}

void MainMenuGUI::on_pushButton_2_clicked()
{
    newTank *form = new newTank;
    form->show();
}

void MainMenuGUI::on_pushButton_3_clicked()
{
    r_Costs *form = new r_Costs;
    form->show();
}

void MainMenuGUI::on_pushButton_4_clicked()
{
    newRepair *form = new newRepair;
    form->show();
}

void MainMenuGUI::on_pushButton_5_clicked()
{
    r_Workshop *form = new r_Workshop;
    form->show();
}

void MainMenuGUI::on_pushButton_6_clicked()
{
    admin *form_admin = new admin();
    form_admin->show();
}

void MainMenuGUI::on_pushButton_7_clicked()
{
    ed_user *form = new ed_user();
    form->show();
}

void MainMenuGUI::on_pushButton_8_clicked()
{
    reg_user *form_reg = new reg_user();
    form_reg->show();
}
