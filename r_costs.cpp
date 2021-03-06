#include "r_costs.h"
#include "ui_r_costs.h"

#include <QDebug>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QtPrintSupport>

r_Costs::r_Costs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::r_Costs)
{
    ui->setupUi(this);

    connector = new SQL();

//    series = new QAreaSeries();
    series0 = new QLineSeries();
    series1 = new QLineSeries();
    series2 = new QLineSeries();
    series3 = new QLineSeries();

    barset0 = new QBarSet("ON");
    barset1 = new QBarSet("Pb");
    barset2 = new QBarSet("LPG");

    axisX = new QCategoryAxis();
    axisY = new QCategoryAxis();

    mainChart = new QChart();
    chart0init();

    barChart = new QChart();
    chart1init();

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_2->setDate(QDate::currentDate());
    ui->gridLayoutWidget_2->setVisible(false);

}

r_Costs::~r_Costs()
{
    delete ui;
}

void r_Costs::loadComboBox(int tanktype, QComboBox *combo)
{
    combo->clear();
    QString stream;
    int i=0;

    if(tanktype==0)
    {
        combo->clear();
        connector->tankType(0); // dla diesla tu musi być 0
        while (connector->getCarName(stream,i)) {
          combo->addItem(stream,i);
    }
    }
    else if(tanktype==1)
    {
         combo->clear();
         connector->tankType(1); // dla Pb tu musi być 1
         while (connector->getCarName(stream,i)) {
         combo->addItem(stream,i);}
         connector->tankType(2);
         while(connector->getCarName(stream,i)){ // dołożenie aut Pb+LPG
         combo->addItem(stream,i);}
    }
    else if(tanktype==2)
    {
          combo->clear();
          connector->tankType(2); // dla Pb+LPG tu musi być 2
          while (connector->getCarName(stream,i)) {
          combo->addItem(stream,i);
    }
    }
    else
      qDebug() << "Błąd wyboru baku";


        ui->cb_carid->update();

}

void r_Costs::loadSeries(QLineSeries *seriesN, int carId, int tankType, float &temp_ymin, float &temp_ymax, int elderyear, int nowyear)
{
    QString date;

    int fuelId;
    float fuelCon=0;
    float month =0;
    float temp_day =0;
    float temp_day_v =0;

    SQL *lS = new SQL;
    lS->fuelInfo(carId,elderyear, nowyear, tankType);

    seriesN->clear();

    while(lS->fuelInfoQuest(fuelId,date,fuelCon))
    {
        QDate temp_date;
        temp_date = QDate::fromString(date,"yyyy-MM-dd");
        temp_day = temp_date.day();
        temp_day_v = temp_day*0.03;
        month = temp_date.month() ;

        if(fuelCon>0.0)
        {
            seriesN->append(((month-1)+temp_day_v),fuelCon);
            setyMaxMin(fuelCon,temp_ymin,temp_ymax);
        }
    }
}

void r_Costs::setXAxis()
{    
    axisX->append("Styczeń", 1);
    axisX->append("Luty", 2);
    axisX->append("Marzec", 3);
    axisX->append("Kwiecień",4);
    axisX->append("Maj",5);
    axisX->append("Czerwiec",6);
    axisX->append("Lipiec",7);
    axisX->append("Sierpień",8);
    axisX->append("Wrzesień",9);
    axisX->append("Październik",10);
    axisX->append("Listopad",11);
    axisX->append("Grudzień",12);

    mainChart->axisX()->setRange(0,12);
}

void r_Costs::setYAxis()
{
    axisY->setRange(yMin,yMax);
}

void r_Costs::setyMinMax(float v)
{
    if(v<=yMin)
    {
        yMin=v;
    }
    else if(v>=yMax)
    {
        yMax=v;
    }
}

void r_Costs::setyMinMax(float yMin1, float yMax1, float yMin2, float yMax2)
{
    if(yMin<yMin1 && yMin<yMin2)
    {
        if(yMin1<yMin2)
        {
            yMin=yMin1;
        }
        else
        {
            yMin=yMin2;
        }
    }
    else
    {
        if(yMin1<yMin2)
        {
            yMin=yMin1;
        }
        else
        {
            yMin=yMin2;
        }
    }

    if(yMax>yMax1 && yMax>yMax2)
    {
        if(yMax1>yMax2)
        {
            yMax=yMax1;
        }
        else
        {
            yMax=yMax2;
        }
    }
    else
    {
        if(yMax1>yMax2)
        {
            yMax=yMax1;
        }
        else
        {
            yMax=yMax2;
        }
    }
}

void r_Costs::setyMinMax(float yMin1, float yMax1, float yMin2, float yMax2, float yMin3, float yMax3)
{
//    qDebug() << "====== CZARY!! =============";

//    qDebug() << "ymax: " + QString::number(yMax);
//    qDebug() << "ymin: " + QString::number(yMin);

//    qDebug() << "ymax1: " + QString::number(yMax1);
//    qDebug() << "ymin1: " + QString::number(yMin1);

//    qDebug() << "ymax2: " + QString::number(yMax2);
//    qDebug() << "ymin2: " + QString::number(yMin2);

//    qDebug() << "ymax3: " + QString::number(yMax3);
//    qDebug() << "ymin3: " + QString::number(yMin3);

//    qDebug() << "====== CZARY!! =============";


    if(yMin<yMin1 && yMin<yMin2 && yMin<yMin3)
    {

        float temp=0;
        if(yMin1<yMin2)
        {
            temp=yMin1;
        }
        else
        {
            temp=yMin2;
        }

        if(temp<yMin3)
        {
            yMin=temp;
        }
        else
        {
            yMin=yMin3;
        }
    }
    else
    {
        float temp=0;
        if(yMin1<yMin2)
        {
            temp=yMin1;
        }
        else
        {
            temp=yMin2;
        }

        if(temp<yMin3)
        {
            yMin=temp;
        }
        else
        {
            yMin=yMin3;
        }
    }

    if(yMax>yMax1 && yMax>yMax2 && yMax>yMax3)
    {
        float temp=0;
        if(yMax1>yMax2)
        {
            temp = yMax1;
        }
        else
        {
            temp = yMax2;
        }

        if(temp>yMax3)
        {
            yMax=temp;
        }
        else
        {
            yMax= yMax3;
        }
 //       qDebug() << "YMAX!!: " + QString::number(yMax);
    }
    else
    {
        float temp=0;
        if(yMax1>yMax2)
        {
            temp = yMax1;
        }
        else
        {
            temp = yMax2;
        }

        if(temp>yMax3)
        {
            yMax=temp;
        }
        else
        {
            yMax= yMax3;
        }
//        qDebug() << "YMAX!!: " + QString::number(yMax);
    }
}

void r_Costs::setyMaxMin(float v, float &tempyMin, float &tempyMax)
{
    if(v<=tempyMin)
    {
        tempyMin=v;
//        qDebug() << "TempMAx:" + QString::number(tempyMin);
    }
    else if(v>=tempyMax)
    {
        tempyMax=v;
//        qDebug() << "TempMIx:" + QString::number(tempyMax);
    }
}

void r_Costs::loadBars(QBarSet *barsetN, int elderyear, int nowyear, int fuelType, int typeChart, int carID, float &temp_ymin, float &temp_ymax)
{
    SQL *bS = new SQL;

    if(typeChart==1)
    {
        int i=1;
        while (i<13) {
            *barsetN << (bS->fuelsCosts(fuelType,elderyear,nowyear,i,carID));
            setyMaxMin(bS->fuelsCosts(fuelType,elderyear,nowyear,i,carID),temp_ymin,temp_ymax);
            ++i;
        }
    }
    else
    {
        int i=1;
        while(i<13)
        {
            *barsetN <<(bS->fuelQuantity(fuelType,elderyear,nowyear,i,carID));
            setyMaxMin(bS->fuelQuantity(fuelType,elderyear,nowyear,i,carID),temp_ymin,temp_ymax);
            ++i;
        }
    }
}

void r_Costs::on_chb_on_clicked()
{
    ui->gridLayoutWidget_2->setVisible(false);
    ui->cb_carid_2->clear();
    ui->cb_carid_3->clear();
    ui->cb_carid_2->setVisible(false);
    ui->cb_carid_3->setVisible(false);
    ui->l_carname3->setVisible(false);
    ui->chb_lpg->setChecked(false);
    ui->chb_pb->setChecked(false);
    tankTyp = 0;
    loadComboBox(0,ui->cb_carid);
}

void r_Costs::on_chb_pb_clicked()
{
    ui->gridLayoutWidget_2->setVisible(false);
    ui->cb_carid_2->clear();
    ui->cb_carid_3->clear();
    ui->cb_carid_2->setVisible(false);
    ui->cb_carid_3->setVisible(false);
    ui->l_carname3->setVisible(false);
    ui->chb_on->setChecked(false);
    ui->chb_lpg->setChecked(false);
    tankTyp = 1;
    loadComboBox(1,ui->cb_carid);
}

void r_Costs::on_chb_lpg_clicked()
{
    ui->gridLayoutWidget_2->setVisible(false);
    ui->cb_carid_2->clear();
    ui->cb_carid_3->clear();
    ui->cb_carid_2->setVisible(false);
    ui->cb_carid_3->setVisible(false);
    ui->l_carname3->setVisible(false);
    ui->chb_on->setChecked(false);
    ui->chb_pb->setChecked(false);
    tankTyp = 2;
    loadComboBox(2,ui->cb_carid);
}

void r_Costs::on_pushButton_clicked()
{

    if((ui->cb_carid_2->isVisible() == false))
    {
        ui->gridLayoutWidget_2->setVisible(true);
        ui->cb_carid_2->setVisible(true);
        if(ui->chb_on->isChecked())
        {
            loadComboBox(0,ui->cb_carid_2);
        }
        else if(ui->chb_pb->isChecked())
        {
            loadComboBox(1,ui->cb_carid_2);
        }
        else if(ui->chb_lpg->isChecked())
        {
            loadComboBox(2,ui->cb_carid_2);
        }
        else
            qDebug() << "Bład wyboru baku";


        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series1);


    }
    else if(ui->cb_carid_2->isVisible())
    {
        ui->l_carname3->setVisible(true);
        ui->cb_carid_3->setVisible(true);
        if(ui->chb_on->isChecked())
        {
            loadComboBox(0,ui->cb_carid_3);
        }
        else if(ui->chb_pb->isChecked())
        {
            loadComboBox(1,ui->cb_carid_3);
        }
        else if(ui->chb_lpg->isChecked())
        {
            loadComboBox(2,ui->cb_carid_3);
        }
        else
            qDebug() << "Bład wyboru baku";

        //mainChart->addSeries(series2);
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series2);
    }
}

void r_Costs::on_cb_carid_currentIndexChanged(const QString &arg1)
{
    QDate today;
    int td = today.currentDate().year();


    SQL *tempCon = new SQL;

    int i;
    i = ui->cb_carid->currentData().toInt();

    tmp_yMin1 = 0; tmp_yMax1=1;

    if(elder==-1 || now==-1)
    {
        loadSeries(series0,i,tankTyp,tmp_yMin1,tmp_yMax1,td,td);
    }
    else
    {
        loadSeries(series0,i,tankTyp,tmp_yMin1,tmp_yMax1,elder,now);
    }

    setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
    series0->setName(ui->cb_carid->currentText());
    mainChart->removeAxis(axisX);
    mainChart->createDefaultAxes();
    mainChart->setAxisX(axisX,series0);
    mainChart->axisY()->setRange(yMin,yMax+0.5);
    chartView->repaint();
    ui->widget->update();

    tempCon->fuelInfo(i);

    barset0->remove(0,12);

    temp_yMax1=1; temp_yMin1=0;

    if(elder==-1 || now==-1)
    {
        loadBars(barset0,td,td,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin1,temp_yMax1);

    }
    else
    {
        loadBars(barset0,elder,now,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin1,temp_yMax1);
    }

    barset0->setLabel(ui->cb_carid->currentText());
    setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
    barChart->axisY()->setRange(yMin,yMax);
    barChartView->repaint();
    ui->widget_2->update();
}

void r_Costs::on_cb_carid_2_currentIndexChanged(const QString &arg1)
{
    QDate today;
    int td = today.currentDate().year();

    SQL *tempCon = new SQL;

    int i;
    i = ui->cb_carid_2->currentData().toInt();

    tmp_yMin2 = 0; tmp_yMax2=1;

    if(elder==-1 || now==-1)
    {
        loadSeries(series1,i,tankTyp,tmp_yMin2,tmp_yMax2,td,td);
    }
    else
    {
        loadSeries(series1,i,tankTyp,tmp_yMin2,tmp_yMax2,elder,now);
    }

    setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
    series1->setName(ui->cb_carid_2->currentText());
    mainChart->removeAxis(axisX);
    mainChart->createDefaultAxes();
    mainChart->setAxisX(axisX,series1);
    mainChart->axisY()->setRange(yMin,yMax+0.5);
    chartView->repaint(); // <-- tutaj ma ładować 2 serie
    ui->widget->update();

    tempCon->fuelInfo(i);

    barset1->remove(0,12);

    temp_yMax2=1; temp_yMin2=0;

    if(elder==-1 || now==-1)
    {
        loadBars(barset1,td,td,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin2,temp_yMax2);
    }
    else
    {
        loadBars(barset1,elder,now,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin2,temp_yMax2);
    }

    barset1->setLabel(ui->cb_carid_2->currentText());
    setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
    barChart->axisY()->setRange(yMin,yMax);
    //barChart->setTitle("Zakup paliw w Litrach");
    barChartView->repaint();
    ui->widget_2->update();
}


void r_Costs::on_cb_carid_3_currentIndexChanged(const QString &arg1)
{
    QDate today;
    int td = today.currentDate().year();

    SQL *tempCon = new SQL;

    int i;
    i = ui->cb_carid_3->currentData().toInt();

    tmp_yMin3 = 0; tmp_yMax3=1;
    if(elder==-1 || now==-1)
    {
        loadSeries(series2,i,tankTyp,tmp_yMin3,tmp_yMax3,td,td);
    }
    else
    {
        loadSeries(series2,i,tankTyp,tmp_yMin3,tmp_yMax3,elder,now);
    }

    setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
    series2->setName(ui->cb_carid_3->currentText());
    mainChart->removeAxis(axisX);
    mainChart->createDefaultAxes();
    mainChart->setAxisX(axisX,series2);
    mainChart->axisY()->setRange(yMin,yMax+0.5);
    chartView->repaint(); // <-- tutaj ma ładować 3 serie
    ui->widget->update();

    tempCon->fuelInfo(i);

    barset2->remove(0,12);

    temp_yMax3=1; temp_yMin3=0;

    if(elder==-1 || now==-1)
    {
        loadBars(barset2,td,td,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin3,temp_yMax3);
    }
    else
    {
        loadBars(barset2,elder,now,tempCon->fuelInfoQuest(),typeChart,i,temp_yMin3,temp_yMax3);
    }

    barset2->setLabel(ui->cb_carid_3->currentText());
    setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
    barChart->axisY()->setRange(yMin,yMax);
    //barChart->setTitle("Zakup paliw w Litrach");
    barChartView->repaint();
    ui->widget_2->update();

}

void r_Costs::on_pushButton_2_clicked()
{
    ui->widget->grab().save("image.jpg");
}

void r_Costs::chart0init()
{
    mainChart->addSeries(series0);
    mainChart->addSeries(series1);
    mainChart->addSeries(series2);
    mainChart->createDefaultAxes();
    mainChart->legend()->setVisible(true);
    mainChart->setTitle("Średnia spalania w ciągu roku");
    setXAxis();
    chartView = new QChartView(mainChart, ui->widget);
    chartView->setRenderHint(QPainter::Antialiasing);
    //chartView->chart()->setTheme(QChart::ChartThemeBlueNcs);
    chartView->resize((ui->widget->width()-10),(ui->widget->height()-10));
}

void r_Costs::chart1init()
{
    seriesX = new QBarSeries();
    seriesX->append(barset0);
    seriesX->append(barset1);
    seriesX->append(barset2);

    barChart->setTitle("Zakup paliw w PLN");
    barChart->addSeries(seriesX);

    barChartView = new QChartView(barChart,ui->widget_2);
    barChartView->setRenderHint(QPainter::Antialiasing);
    barChartView->resize((ui->widget_2->width()-10),(ui->widget_2->height()-10));

    QStringList categories;
    categories << "Styczeń" << "Luty" << "Marzec" << "Kwiecień" << "Maj" << "Czerwiec" << "Lipiec" << "Sierpień"
               << "Wrzesień" << "Październik" << "Listopad" << "Grudzień";

    axis = new QBarCategoryAxis();
    axis->append(categories);
    barChart->createDefaultAxes();
    barChart->setAxisX(axis, seriesX);

    barChartView->repaint();
    ui->widget_2->repaint();
    ui->widget_2->update();
}


void r_Costs::on_b_search_clicked()
{
    barset0->remove(0,12);
    barset1->remove(0,12);
    barset2->remove(0,12);

    int elder = ui->dateEdit->date().year();
    int younger = ui->dateEdit_2->date().year();
    now = younger;
    typeChart = 1;
    if(younger-elder<0)
    {
        QMessageBox::information(0,"Błąd","Data 'Od' jest młodsza od daty 'Do'");
    }
    else
    {
        SQL *tempCon = new SQL;


        tmp_yMin1 = 0; tmp_yMax1=1;
        loadSeries(series0,ui->cb_carid->currentData().toInt(),tankTyp,tmp_yMin1,tmp_yMax1,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series0->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series0);
        mainChart->axisY()->setRange(yMin,yMax+0.5);

        tmp_yMin2 = 0; tmp_yMax2=1;
        loadSeries(series1,ui->cb_carid_2->currentData().toInt(),tankTyp,tmp_yMin2,tmp_yMax2,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series1->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series1);
        mainChart->axisY()->setRange(yMin,yMax+0.5);

        tmp_yMin3 = 0; tmp_yMax3=1;
        loadSeries(series2,ui->cb_carid_3->currentData().toInt(),tankTyp,tmp_yMin3,tmp_yMax3,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series2->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series3);
        mainChart->axisY()->setRange(yMin,yMax+0.5);
        chartView->repaint();
        ui->widget->update();

        tmp_yMin1 = 0; tmp_yMax1=1;
        loadSeries(series0,ui->cb_carid->currentData().toInt(),tankTyp,tmp_yMin1,tmp_yMax1,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series0->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series0);
        mainChart->axisY()->setRange(yMin,yMax+0.5);
        chartView->repaint();
        ui->widget->update();


        temp_yMax1=1; temp_yMin1=0;
        tempCon->fuelInfo(ui->cb_carid->currentData().toInt());
        loadBars(barset0,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid->currentData().toInt(),temp_yMin1,temp_yMax1);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        temp_yMax2=1; temp_yMin2=0;
        tempCon->fuelInfo(ui->cb_carid_2->currentData().toInt());
        loadBars(barset1,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid_2->currentData().toInt(),temp_yMin2,temp_yMax2);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        temp_yMax3=1; temp_yMin3=0;
        tempCon->fuelInfo(ui->cb_carid_3->currentData().toInt());
        loadBars(barset2,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid_3->currentData().toInt(),temp_yMin3,temp_yMax3);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        barChart->setTitle("Zakup paliw w PLN");
        barChartView->repaint();
        ui->widget_2->repaint();
        ui->widget_2->update();
    }


}

void r_Costs::on_b_quantity_clicked()
{
    barset0->remove(0,12);
    barset1->remove(0,12);
    barset2->remove(0,12);

    int elder = ui->dateEdit->date().year();
    int younger = ui->dateEdit_2->date().year();
    now = younger;
    typeChart = 2;

    if(younger-elder<0)
    {
        QMessageBox::information(0,"Błąd","Data 'Od' jest młodsza od daty 'Do'");
    }
    else
    {
        SQL *tempCon = new SQL;


        tmp_yMin1 = 0; tmp_yMax1=1;
        loadSeries(series0,ui->cb_carid->currentData().toInt(),tankTyp,tmp_yMin1,tmp_yMax1,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series0->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series0);
        mainChart->axisY()->setRange(yMin,yMax+0.5);

        tmp_yMin2 = 0; tmp_yMax2=1;
        loadSeries(series1,ui->cb_carid_2->currentData().toInt(),tankTyp,tmp_yMin2,tmp_yMax2,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series1->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series1);
        mainChart->axisY()->setRange(yMin,yMax+0.5);

        tmp_yMin3 = 0; tmp_yMax3=1;
        loadSeries(series2,ui->cb_carid_3->currentData().toInt(),tankTyp,tmp_yMin3,tmp_yMax3,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series2->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series3);
        mainChart->axisY()->setRange(yMin,yMax+0.5);
        chartView->repaint();
        ui->widget->update();

        tmp_yMin1 = 0; tmp_yMax1=1;
        loadSeries(series0,ui->cb_carid->currentData().toInt(),tankTyp,tmp_yMin1,tmp_yMax1,elder,younger);
        setyMinMax(tmp_yMin1,tmp_yMax1,tmp_yMin2,tmp_yMax2,tmp_yMin3,tmp_yMax3);
        series0->setName(ui->cb_carid->currentText());
        mainChart->removeAxis(axisX);
        mainChart->createDefaultAxes();
        mainChart->setAxisX(axisX,series0);
        mainChart->axisY()->setRange(yMin,yMax+0.5);
        chartView->repaint();
        ui->widget->update();


        temp_yMax1=1; temp_yMin1=0;
        tempCon->fuelInfo(ui->cb_carid->currentData().toInt());
        loadBars(barset0,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid->currentData().toInt(),temp_yMin1,temp_yMax1);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        temp_yMax2=1; temp_yMin2=0;
        tempCon->fuelInfo(ui->cb_carid_2->currentData().toInt());
        loadBars(barset1,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid_2->currentData().toInt(),temp_yMin2,temp_yMax2);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        temp_yMax3=1; temp_yMin3=0;
        tempCon->fuelInfo(ui->cb_carid_3->currentData().toInt());
        loadBars(barset2,elder,younger,tempCon->fuelInfoQuest(),typeChart,ui->cb_carid_3->currentData().toInt(),temp_yMin3,temp_yMax3);
        setyMinMax(temp_yMin1,temp_yMax1,temp_yMin2,temp_yMax2,temp_yMin3,temp_yMax3);
        barChart->axisY()->setRange(yMin,yMax);

        barChart->setTitle("Zakup paliw w Litrach");
        barChartView->repaint();
        ui->widget_2->repaint();
        ui->widget_2->update();
    }


}

void r_Costs::on_pushButton_3_clicked()
{
    //ui->widget_2->grab().save("image2.jpg");
    //ui->widget_3->resize();
    //ui->widget_3->grab().save("imageTest.jpg");

   if(ui->le_raporttitle->text().length()>0)
   {
       //ui->widget_2->grab().save("image2.jpg");
          //ui->widget_3->resize();
          ui->widget_3->grab().save("imageTest.jpg");

         QTextDocument doc;
         doc.setDefaultFont(QFont("Times",14));

         QTextBlockFormat tbf; // Text na środku
         QTextBlockFormat tbf1; // Łamacz stron
         QTextBlockFormat tbf2; // Text do lewej

         tbf.setAlignment(Qt::AlignCenter);
         tbf.setLineHeight(2,QTextBlockFormat::LineDistanceHeight);
         tbf1.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysAfter);
         tbf2.setAlignment(Qt::AlignLeft);

         QTextCursor tc(&doc);
         tc.setBlockFormat(tbf);
         //QImage image2(":/images/images/data-analysis-symbol.png");

         QTextCharFormat tcf;
         int p = 32;
         qreal Psize = p;
         tcf.setFontPointSize(Psize);

         //tc.insertImage(image2.scaled(512,512,Qt::KeepAspectRatio),"Test");
         tc.insertText("\n");
         tc.mergeCharFormat(tcf);
         tc.insertText(ui->le_raporttitle->text());

         tc.mergeBlockFormat(tbf1);
         tc.setBlockFormat(tbf);

         auto temp = ui->widget_3->grab();

         QImage image = temp.toImage();
         image.scaled(200,70,Qt::KeepAspectRatio);
         tc.insertImage(image,QString("Raport"));
         tc.insertText("\n");
         tc.insertText("\n");
         tc.insertText("\n");

         tc.mergeBlockFormat(tbf1);
         tc.setBlockFormat(tbf2);

         tc.insertText(ui->plainTextEdit->toPlainText());

         //tc.mergeBlockFormat(tbf1);
         //tc.setBlockFormat(tbf);
         //tc.insertText("Jakiś tekst");

         QPrinter printer(QPrinter::HighResolution); //= new QPrinter(QPrinter::HighResolution);
                  printer.setOutputFormat(QPrinter::PdfFormat);
                  printer.setOutputFileName("output.pdf");
                  printer.setPaperSize(QSizeF(297,210),QPrinter::Millimeter);
                  printer.setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);
                  doc.print(&printer);
   }
   else
   {
       QMessageBox::information(0,"Błąd","Wprowadź tytuł raportu");
   }

}

void r_Costs::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
            chartView->setRubberBand(QChartView::HorizontalRubberBand);
            break;
        case Qt::Key_Minus:
            mainChart->zoomReset();
        break;
    }
}
