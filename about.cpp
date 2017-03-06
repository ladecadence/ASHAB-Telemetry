#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->textBrowser->setStyleSheet("background: transparent;");
    ui->graphicsView->setStyleSheet("background: transparent;");
    QPixmap pixmap(":/Images/images/ashab-small.png");
    QGraphicsScene *scn = new QGraphicsScene(); // object defined in header
    ui->graphicsView->setScene(scn);
    scn->addPixmap(pixmap);
    ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);
    ui->labelBuild->setText(QString::fromLocal8Bit(__DATE__));
}

About::~About()
{
    delete ui;
}
