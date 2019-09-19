#include "chartsdialog.h"
#include "ui_chartsdialog.h"

ChartsDialog::ChartsDialog(QWidget *parent, LogDialog *log) :
    QDialog(parent),
    ui(new Ui::ChartsDialog)
{
    ui->setupUi(this);
    logDialog = log;

    // init chart
    chart = new QChart();
    lineSeries = new QLineSeries();
    chart->addSeries(lineSeries);
    //axisX = new QDateTimeAxis;
    //axisY = new QValueAxis;

    chartTitles = new QStringList();
    chartValues = new QStringList();

    // names and values
    // chart names
    *chartTitles << "" << "" << "" << "" << "Altitud"
                << "Batería" << "Temperatura Interna"
                << "Temperatura Externa" << "Barómetro" << ""
                << "" << "Satélites GPS"
                << "Ratio de ascenso";

    *chartValues << "" << "" << "" << "" << "m"
                << "V" << "ºC" << "ºC" <<"mBar"
                << "" << "" << "#" << "m/s";

    // create first graph
    chart_type = LOG_ALT;
    ui->radioButtonAltitude->toggle();
    update_chart();

}

ChartsDialog::~ChartsDialog()
{
    delete chart;
    delete chartValues;
    delete chartTitles;
    if (lineSeries->attachedAxes().length() > 0) {
        delete axisX;
        delete axisY;
    }
    delete lineSeries;
    delete ui;
}

void ChartsDialog::on_radioButtonAltitude_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_ALT)->join(":");
        chart_type = LOG_ALT;
        update_chart();
    }
}

void ChartsDialog::on_radioButtonTint_toggled(bool checked)
{
    if (checked) {
        chart_type = LOG_TINT;
        update_chart();
    }
}


void ChartsDialog::on_radioButtonText_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_TEXT)->join(":");
        chart_type = LOG_TOUT;
        update_chart();
    }
}


void ChartsDialog::on_radioButtonBaro_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_BARO)->join(":");
        chart_type = LOG_BARO;
        update_chart();
    }
}


void ChartsDialog::on_radioButtonBatt_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_BATT)->join(":");
        chart_type = LOG_BATT;
        update_chart();
    }
}


void ChartsDialog::on_radioButtonSats_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_SATS)->join(":");
        chart_type = LOG_SATS;
        update_chart();
    }
}

void ChartsDialog::on_radioButtonArate_toggled(bool checked)
{
    if (checked) {
        //qDebug() << logDialog->get_data(LOG_SATS)->join(":");
        chart_type = LOG_ARATE;
        update_chart();
    }
}

void ChartsDialog::update_chart()
{
    // start cleaning stuff
    if (lineSeries->attachedAxes().length() > 0) {
        lineSeries->detachAxis(axisX);
        lineSeries->detachAxis(axisY);
        delete axisX;
        delete axisY;
    }
    lineSeries->clear();
    chart->removeSeries(lineSeries);

    // Get new data and import it to the lines graph
    QStringList* data = new QStringList;
    logDialog->get_data(chart_type, data);
    QStringList* dates = new QStringList;
    logDialog->get_data(LOG_DATE, dates);
    QStringList* times = new QStringList;
    logDialog->get_data(LOG_HOUR, times);

    for (int i=0; i<data->length(); i++) {
            //qDebug() << data->at(i);
            QDateTime momentInTime;
            QDate date = QDate::fromString(dates->at(i), "dd-MM-yyyy");
            QTime time = QTime::fromString(times->at(i), "hh:mm:ss");
            momentInTime.setDate(date);
            momentInTime.setTime(time);
            lineSeries->append(momentInTime.toMSecsSinceEpoch(),
                               static_cast<qreal>(data->at(i).toFloat()));
    }

    // now configure graph
    chart->addSeries(lineSeries);
    chart->legend()->hide();

    // Axes
    axisX = new QDateTimeAxis;
    axisX->setTickCount(10);
    axisX->setFormat("dd'd' hh:mm:ss");
    axisX->setTitleText("Date");
    axisX->setLabelsAngle(90);
    chart->addAxis(axisX, Qt::AlignBottom);
    lineSeries->attachAxis(axisX);

    axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText(chartValues->at(chart_type));
    chart->addAxis(axisY, Qt::AlignLeft);
    lineSeries->attachAxis(axisY);

    chart->setTitle(chartTitles->at(chart_type));

    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    delete data;
    delete dates;
    delete times;

}



void ChartsDialog::on_saveButton_clicked()
{
    QString path;

    // Take file path and name that will create
    QString newPath = QFileDialog::getSaveFileName(this,
                                                   trUtf8("Save SVG"),
                                                   path,
                                                   tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

    QSvgGenerator generator;        // Create a file generator object
    generator.setFileName(path);    // We set the path to the file
                                    // where to save vector graphics
    // Set the dimensions of the working area of the document in millimeters
    generator.setSize(
                QSize(static_cast<int>(ui->chartView->scene()->width()),
                      static_cast<int>(ui->chartView->scene()->height())));
    // Set the work area in the coordinates
    generator.setViewBox(
                QRect(0, 0, static_cast<int>(ui->chartView->scene()->width()),
                      static_cast<int>(ui->chartView->scene()->height())));
    generator.setTitle(trUtf8("SVG Example"));  // The title document
    generator.setDescription(trUtf8("File created by SVG Example"));

    QPainter painter;
    painter.begin(&generator);
    ui->chartView->scene()->render(&painter);
    painter.end();

}
