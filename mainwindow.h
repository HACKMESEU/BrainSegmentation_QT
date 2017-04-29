#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
//#include "dcmtk/config/osconfig.h"
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>


#include "segment.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QImage MatToQImage(const cv::Mat& mat);
    void ProcessAndShowImage(QString fileName);
    void ProcessAndSaveImage(QString fileName,QString);
    void PrintMat(Mat mat, int rows,int cols);


public slots:
    void OpenFile();
    void OpenFiles();
    void ChangeImage();
    void Saveimages();

private:
    Ui::MainWindow *ui;
    QFileInfoList list;
    QStringList file_list;
    QString OutputPath;
};

#endif // MAINWINDOW_H
