#include "ssdvpicturedialog.h"
#include "ui_ssdvpicturedialog.h"

SSDVPictureDialog::SSDVPictureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SSDVPictureDialog)
{
    ui->setupUi(this);
}

SSDVPictureDialog::~SSDVPictureDialog()
{
    delete ui;
}

void SSDVPictureDialog::on_buttonBox_accepted()
{

}

void SSDVPictureDialog::loadPicture(QString picturePath)
{
    ui->labelPath->setText(picturePath);
    QPixmap pixmap(picturePath);
    QGraphicsScene *scn = new QGraphicsScene(); // object defined in header
    ui->graphicsView->setScene(scn);
    scn->addPixmap(pixmap);
    //ui->graphicsView->fitInView(scn->itemsBoundingRect(),Qt::KeepAspectRatio);

}
