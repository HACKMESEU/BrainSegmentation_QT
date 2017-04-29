#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionOpen_File,SIGNAL(triggered()),this,SLOT(OpenFile()));
    connect(ui->actionOpen_Files,SIGNAL(triggered()),this,SLOT(OpenFiles()));
    connect(ui->verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(ChangeImage()));
    connect(ui->pushButton_save,SIGNAL(clicked(bool)),this,SLOT(Saveimages()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::OpenFile()
{

    QString fileName = QFileDialog::getOpenFileName(this);
    ProcessAndShowImage(fileName);
    //return 0;
}

void MainWindow::ProcessAndShowImage(QString fileName){
    if(!fileName.isEmpty())
    {

//    DicomImage *image = new DicomImage("C:/Users/dell/Desktop/BrainSegmentation/S3010/I20");
       DicomImage *image = new DicomImage(fileName.toLatin1());
//    if(image != NULL)
//    {
//        auto height=image->getHeight();
//        auto width=image->getWidth();
//        std::cout<<image->getHeight()<<" "<<image->getWidth()<<std::endl;
//        auto isChom=image->isMonochrome();
//        std::cout<<"is mono chrom:"<<image->isMonochrome()<<std::endl;
//    }
    /* Get the pixel data from Dicom file. */
    const DiPixel *const interData =  (image->getInterData());

    /* Determine the pixel representation. */
    EP_Representation pixelRep = image->getInterData()->getRepresentation();
    size_t scalarSize ;
    switch(pixelRep)
      {
      case EPR_Uint8:
        scalarSize = sizeof(unsigned char); break;
      case EPR_Sint8:
        scalarSize = sizeof(char); break;
      case EPR_Uint16:
        scalarSize = sizeof(unsigned short); break;
      case EPR_Sint16:
        scalarSize = sizeof(short); break;
      case EPR_Uint32:
        scalarSize = sizeof(unsigned int); break;
      case EPR_Sint32:
        scalarSize = sizeof(int); break;
      default:
        scalarSize = sizeof(unsigned short); break;
      }

    /* Create a buffer to put the pixel data. */
    Uint16 *buffer;
    buffer= new Uint16[image->getHeight()*image->getWidth()];
    memcpy(buffer,
               interData->getData(),
               interData->getCount()*scalarSize);

   /* Change the buffer into Mat format. */
    Mat image2(int(image->getHeight()),int(image->getWidth()),CV_16S,buffer);

    /* Put the origin dicom image in the UI. */
    Mat image8U;
    image2.convertTo(image8U,CV_8U);
    QImage imageorigin = MatToQImage(image8U);
    QGraphicsScene *scene1 = new QGraphicsScene;
    scene1->addPixmap(QPixmap::fromImage(imageorigin));
    ui->graphicsView_Origin->setScene(scene1);
    ui->graphicsView_Origin->resize(imageorigin.width()+10, imageorigin.height()+10);
    ui->graphicsView_Origin->show();



    /* Start the segmentation prrocedure. */
    Mat ROIs;
    ROIs = Segmentation(image2);
    //imshow("roi",ROIs);

    /* Remove the pixel not intersted. */
    for(int i=0;i<image->getWidth();i++)
    {
       for(int j=0;j<image->getHeight();j++)
        {
             if(*(ROIs.data + ROIs.step[0] * i + ROIs.step[1] * j) == 0)
             {
                    buffer[i*image->getHeight()+j] =0;
             }
        }
    }

    /* Show the segmentation result in UI. */
    Mat image_result(int(image->getHeight()),int(image->getWidth()),CV_16S,buffer);
    image_result.convertTo(image8U,CV_8U);
    QImage imageresult = MatToQImage(image8U);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(imageresult));
    ui->graphicsView_Seg->setScene(scene);
    ui->graphicsView_Seg->resize(imageresult.width()+10, imageresult.height()+10);
    ui->graphicsView_Seg->show();

//    DcmFileFormat fileformat;
//    fileformat.loadFile((const OFFilename)(fileName.toLatin1()));
//    DcmDataset* dcmdataset = fileformat.getDataset();
//    dcmdataset->putAndInsertUint16Array(DCM_PixelData,( const Uint16*) (buffer),image->getHeight()*image->getWidth(),true);
//    delete buffer;
//    //DcmFileFormat file(dcmdataset);
//    //OFCondition statu = fileformat.saveFile((const OFFilename)(fileName.toLatin1()),EXS_LittleEndianExplicit);
//    OFCondition statu = fileformat.saveFile("test123",EXS_LittleEndianExplicit);
//    if(statu.bad()){
//        std::cout<<"Can not save!";
//        }
    }
}


void MainWindow::ProcessAndSaveImage(QString fileName,QString OutputFileName){
    if(!fileName.isEmpty())
    {

//    DicomImage *image = new DicomImage("C:/Users/dell/Desktop/BrainSegmentation/S3010/I20");
       DicomImage *image = new DicomImage(fileName.toLatin1());
//    if(image != NULL)
//    {
//        auto height=image->getHeight();
//        auto width=image->getWidth();
//        std::cout<<image->getHeight()<<" "<<image->getWidth()<<std::endl;
//        auto isChom=image->isMonochrome();
//        std::cout<<"is mono chrom:"<<image->isMonochrome()<<std::endl;
//    }
    /* Get the pixel data from Dicom file. */
    const DiPixel *const interData =  (image->getInterData());

    /* Determine the pixel representation. */
    EP_Representation pixelRep = image->getInterData()->getRepresentation();
    size_t scalarSize ;
    switch(pixelRep)
      {
      case EPR_Uint8:
        scalarSize = sizeof(unsigned char); break;
      case EPR_Sint8:
        scalarSize = sizeof(char); break;
      case EPR_Uint16:
        scalarSize = sizeof(unsigned short); break;
      case EPR_Sint16:
        scalarSize = sizeof(short); break;
      case EPR_Uint32:
        scalarSize = sizeof(unsigned int); break;
      case EPR_Sint32:
        scalarSize = sizeof(int); break;
      default:
        scalarSize = sizeof(unsigned short); break;
      }

    /* Create a buffer to put the pixel data. */
    Uint16 *buffer;
    buffer= new Uint16[image->getHeight()*image->getWidth()];
    memcpy(buffer,
               interData->getData(),
               interData->getCount()*scalarSize);

   /* Change the buffer into Mat format. */
    Mat image2(int(image->getHeight()),int(image->getWidth()),CV_16S,buffer);

    /* Start the segmentation prrocedure. */
    Mat ROIs;
    ROIs = Segmentation(image2);
    //imshow("roi",ROIs);

    /* Remove the pixel not intersted. */
    for(int i=0;i<image->getWidth();i++)
    {
       for(int j=0;j<image->getHeight();j++)
        {
             if(*(ROIs.data + ROIs.step[0] * i + ROIs.step[1] * j) == 0)
             {
                    buffer[i*image->getHeight()+j] =0;
             }
        }
    }

    DcmFileFormat fileformat;
    fileformat.loadFile((const OFFilename)(fileName.toLatin1()));
    DcmDataset* dcmdataset = fileformat.getDataset();
    dcmdataset->putAndInsertUint16Array(DCM_PixelData,( const Uint16*) (buffer),image->getHeight()*image->getWidth(),true);
    delete buffer;
    //DcmFileFormat file(dcmdataset);
    OFCondition statu = fileformat.saveFile((const OFFilename)(OutputFileName.toLatin1()),EXS_LittleEndianExplicit);
    //OFCondition statu = fileformat.saveFile("test123",EXS_LittleEndianExplicit);
    if(statu.bad()){
        std::cout<<"Can not save!";
        }
    }
}

void MainWindow::OpenFiles(){
    QString path=QFileDialog::getExistingDirectory(NULL, tr("Choose a input folder."),"D:\\",QFileDialog::ShowDirsOnly);
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    list = dir.entryInfoList();
    file_list = dir.entryList();
    //QString temp = file_list.at(0);
    //std::cout << "     Bytes Filename" << list.size()<<std::endl;
    if(list.size())
    {
        ui->verticalScrollBar->setMinimum(1);
        ui->verticalScrollBar->setMaximum(list.size());
        ui->verticalScrollBar->setSingleStep(1);
        ProcessAndShowImage(list.at(0).filePath());
    }
    else{
        QMessageBox::warning(this, tr("warning:"),
                                         tr("The folder is empty.\n"),
                                         QMessageBox::Ok
                                         );
    }



}

void MainWindow::Saveimages(){
    if(list.size())
    {
    OutputPath=QFileDialog::getExistingDirectory(NULL, tr("Choose a output folder."),"D:\\",QFileDialog::ShowDirsOnly);
    QMessageBox::warning(this,tr("info:"),tr("Please wait for a moment and check the output folder!"),QMessageBox::Ok);
   //QString temp = OutputPath+'/'+file_list.at(0);

        for(int i = 0;i<list.size();i++){
        ProcessAndSaveImage(list.at(i).filePath(),OutputPath+'/'+file_list.at(i));
        }
    }
    else{
        QMessageBox::warning(this, tr("warning:"),
                                         tr("Please open a folder,not a single dicom.\n"),
                                         QMessageBox::Ok
                                         );
    }


}

void MainWindow::ChangeImage(){
    int currentImage = ui->verticalScrollBar->value();
    ProcessAndShowImage(list.at(currentImage-1).filePath());
}

QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        //qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        //qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void MainWindow::PrintMat(Mat mat, int rows,int cols){
    IplImage *Imag;
    Imag=&IplImage(mat);
         for(int i=0;i<rows;i++)
         {
             for(int j=0;j<cols;j++)
             {
                 short ImgPixelVal = cvGetReal2D(Imag,i,j);
                 std::cout <<ImgPixelVal<<"  ";
             }
             std::cout <<" "<<std::endl;
         }
}
