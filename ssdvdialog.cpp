#include "ssdvdialog.h"
#include "ui_ssdvdialog.h"

SSDVDialog::SSDVDialog(QWidget *parent, ConsoleDialog *cons) :
    QDialog(parent),
    ui(new Ui::SSDVDialog)
{
    ui->setupUi(this);
    console = cons;

    model = new QStringListModel(this);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    model->setStringList(lista_ssdv_imgs);
    ui->listView->setModel(model);
    ui->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    pictureDialog = new SSDVPictureDialog();
}

SSDVDialog::~SSDVDialog()
{
    pictureDialog->close();
    delete pictureDialog;
    delete ui;
}

void SSDVDialog::updateStatus(QString status)
{
    ui->ssdvStatusLabel->setText(status);
}

void SSDVDialog::addImageSSDV(QString path){
    if (!lista_ssdv_imgs.contains(path))
    {
        lista_ssdv_imgs.append(path);
        model->setStringList(lista_ssdv_imgs);
    }
}

void SSDVDialog::on_listView_doubleClicked(const QModelIndex &index)
{
    // get ssdv file
    QString path = lista_ssdv_imgs[index.row()];

    // decode it
    decodeSSDV(path);

    // show it
    showImage(path.append(".jpg"));

}

void SSDVDialog::decodeSSDV(QString path)
{
    int i=0;
    FILE *fin = stdin;
    FILE *fout = stdout;
    int droptest = 0;
    int errors;
    ssdv_t ssdv;

    uint8_t pkt[SSDV_PKT_SIZE],  *jpeg;
    size_t jpeg_length;

    //callsign[0] = '\0';

    // decoding
    ui->ssdvStatusLabel->setText(QString("Decodificando ").append(QFileInfo(path).fileName()));

    // open files
    fin = fopen(path.toUtf8().data(), "rb");
    if (!fin)
        return;
    fout = fopen(path.append(".jpg").toUtf8().data(), "wb");
    if (!fout)
        return;

    ssdv_dec_init(&ssdv);
    jpeg_length = 1024 * 1024 * 4;
    jpeg = (uint8_t*) malloc(jpeg_length);
    ssdv_dec_set_buffer(&ssdv, jpeg, jpeg_length);
    while(fread(pkt, 1, SSDV_PKT_SIZE, fin) > 0)
    {
        /* Drop % of packets */
        if(droptest && (rand() / (RAND_MAX / 100) < droptest)) continue;

        /* Test the packet is valid */
        if(ssdv_dec_is_packet(pkt, &errors) != 0) continue;

        ssdv_packet_info_t p;

        ssdv_dec_header(&p, pkt);
        console->append(QString::asprintf("Decoded image packet. Callsign: %s, Image ID: %d, Resolution: %dx%d, Packet ID: %d (%d errors corrected)\n"
                            ">> Type: %d, Quality: %d, EOI: %d, MCU Mode: %d, MCU Offset: %d, MCU ID: %d/%d\n",
                    p.callsign_s,
                    p.image_id,
                    p.width,
                    p.height,
                    p.packet_id,
                    errors,
                    p.type,
                    p.quality,
                    p.eoi,
                    p.mcu_mode,
                    p.mcu_offset,
                    p.mcu_id,
                    p.mcu_count
                    ));

        /* Feed it to the decoder */
        ssdv_dec_feed(&ssdv, pkt);
        i++;
    }

    ssdv_dec_get_jpeg(&ssdv, &jpeg, &jpeg_length);
    fwrite(jpeg, 1, jpeg_length, fout);
    fclose(fout);
    free(jpeg);

    ui->ssdvStatusLabel->setText(QString("Decodificados ").append(QString::number(i)).append(" paquetes de ").
                                 append(QFileInfo(path).fileName()));

    updateImage(path);
}

void SSDVDialog::showImage(QString path)
{
    // let's try to show it

    pictureDialog->loadPicture(path);
    pictureDialog->show();

}

void SSDVDialog::updateImage(QString path)
{
    pictureDialog->loadPicture(path);
}

