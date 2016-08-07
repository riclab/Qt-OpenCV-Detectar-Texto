#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * Metodo para obtener la direccion del video
 * @brief MainWindow::SeleccionarVideo
 */
void MainWindow::SeleccionarVideo(){
    // Declara la variable con la ruta del archivo
    QString archivo = QFileDialog::getOpenFileName(this, tr("Abrir Video"),
                                                    "",
                                                    tr("Videos (*.avi *.mp4 *.mov)"));
    //Agrega la ruta del archivo
    ui->labelVideo->setText(archivo);
    ui->radioVideo->setChecked(true);
}

/**
 * Metodo para procesar el video frame a frame si ckecked==true
 * @brief MainWindow::ProcesarVideo
 * @param checked
 *
 */
void MainWindow::ProcesarVideo(bool checked){

    cv::destroyAllWindows(); // Para cerrar todas las ventanas


    cv::Ptr< cv::text::ERFilter> er_filter1 = cv::text::createERFilterNM1(cv::text::loadClassifierNM1(QString("/home/riclab/dev/opencv/opencv_contrib/modules/text/samples/trained_classifierNM1.xml").toStdString().c_str()),16,0.00015f,0.13f,0.2f,true,0.1f);
    cv::Ptr< cv::text::ERFilter> er_filter2 = cv::text::createERFilterNM2(cv::text::loadClassifierNM2(QString("/home/riclab/dev/opencv/opencv_contrib/modules/text/samples/trained_classifierNM2.xml").toStdString().c_str()), 0.5);



    cv::Mat frame; // Frame como array multidimencional





    if(!checked){ // Si !checked detiene el video si no lo procesa
        ui->play->setText("Iniciar video");
        cap.release();
    }else{
        ui->play->setText("Parar video");

        if(ui->radioVideo->isChecked()){ // si el "radio button" esta seleccionado ejecuta el video si no la webcam
            cap.open(ui->labelVideo->text().toStdString().c_str());
        }else{
            cap.open(0);
        }

    }

    while(checked) // bucle hasta que se precione "parar video"
    {
        cap >> frame; // obtiene un nuevo frame del video o camara
        if(frame.empty()) break; // detiene el bucle si elframe esta vacio

        std::vector< cv::Mat> channels;
        cv::text::computeNMChannels(frame, channels);

        int cn = (int)channels.size();
        // Append negative channels to detect ER- (bright regions over dark background)
        for (int c = 0; c < cn - 1; c++)
         channels.push_back(255 - channels[c]);

        std::vector< std::vector< cv::text::ERStat> > regions(channels.size());

        for (int c = 0; c< (int)channels.size(); c++)
        {
         er_filter1->run(channels[c], regions[c]);
         er_filter2->run(channels[c], regions[c]);
        }

        std::vector< std::vector< cv::Vec2i> > region_groups;
        std::vector< cv::Rect> groups_boxes;
        cv::text::erGrouping(frame, channels, regions, region_groups, groups_boxes, cv::text::ERGROUPING_ORIENTATION_HORIZ);

        groupsDraw(frame, groups_boxes);

        cv::namedWindow("Reproductor", cv::WINDOW_KEEPRATIO); // creamos una ventana la cual permita redimencionar
        cv::imshow("Reproductor", frame); // se muestran los frames
        char key = (char) cv::waitKey(20); //espera 20ms por la tecla ESC
        if(key == 27) break; //detiene el bucle
    }

}

void MainWindow::groupsDraw(cv::Mat &src, std::vector< cv::Rect> &groups)
{
 for (int i = (int)groups.size() - 1; i >= 0; i--)
 {
  if (src.type() == CV_8UC3)
   cv::rectangle(src, groups.at(i).tl(), groups.at(i).br(), cv::Scalar(0, 255, 255), 3, 8);
  else
   cv::rectangle(src, groups.at(i).tl(), groups.at(i).br(), cv::Scalar(255), 3, 8);
 }
}

void MainWindow::on_toolButton_clicked()
{
    SeleccionarVideo();
}

void MainWindow::on_actionAbrir_Video_triggered()
{
    SeleccionarVideo();
}



void MainWindow::on_play_toggled(bool checked)
{
    ProcesarVideo(checked);
}
