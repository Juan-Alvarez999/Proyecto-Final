#include "carnet.h"
#include "ui_carnet.h"


Carnet::Carnet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Carnet)
{
    ui->setupUi(this);
    this->setWindowTitle("Datos");
    ui->carnet->setPlaceholderText("Carnet");
    ui->direccion->setPlaceholderText("IPv4");
    ui->puerto->setPlaceholderText("Puerto");
    QStringList titulo;
    ui->datos->setColumnCount(5);
    titulo<<"Velocidad"<<"Frecuencia"<<"Voltaje"<<"Temperatura"<<"Tiempo_op";
    ui->datos->setHorizontalHeaderLabels(titulo);
}


Carnet::~Carnet()
{
    delete ui;
}

void Carnet::on_enviar_clicked()
{
   char* argv[5];
   char carnet[50];
   char ip[50];
   char puerto[50];
   char programa[50];
   char datos[7][50];
   int cont=0,pos=0;
   char estado[50],buffer[200];
   float velocidad=0,frecuencia=0,voltaje=0,temperatura=0,tiempo=0;
   struct tm ts;
   char buf[80];
   time_t tiempo_div;
   int Tv,Tt=0,Tvolt=0,tv1,tv2;
   pid_t proc;
    QString carnet_in=ui->carnet->text();
    QString direccion_in=ui->direccion->text();
    QString puerto_in=ui->puerto->text();
    std::string carnet_string = carnet_in.toStdString();
    std::string direccion_string = direccion_in.toStdString();
    std::string puerto_string = puerto_in.toStdString();
    if((strcmp(direccion_string.c_str(),"186.155.208.171")==0) && (strcmp(puerto_string.c_str(),"65000")==0)){
        strcpy(programa,"./UDP.bin");
        argv[0]=programa;
        strcpy(ip,direccion_string.c_str());
        argv[1]=ip;
        strcpy(carnet,carnet_string.c_str());
        argv[2]=carnet;
        strcpy(puerto,puerto_string.c_str());
        argv[3]=puerto;
        argv[4]=NULL;

        proc=fork();
        if(proc==0){
            execv("/home/alse/Desktop/UDP/UDP.bin",argv);
        }
        else if(proc>0){
            int state;
            proc=wait(&state);
            ifstream file("/home/alse/Desktop/UDP/motor.txt");
            file>>estado>>buffer;
            //std::cout<<estado<<std::endl;
            if(strcmp(estado,"{\"estado\":\"200")==0){
                for(cont=0;buffer[cont]!='\0';cont++){
                    if(buffer[cont]==':'){
                        sscanf(buffer+cont+1,"%[^,]s",datos[pos]);
                        pos++;
                    }
                }
                sscanf(datos[5],"%[^}]s",datos[6]);



                file.close();
                tv1=carnet[1]-'0';
                tv2=carnet[2]-'0';
                Tv=(tv1*10)+tv2;
                Tvolt=carnet[4]-'0';
                Tt=carnet[6]-'0';
                velocidad=atof(datos[1]);
                frecuencia=atof(datos[2]);
                voltaje=atof(datos[3]);
                temperatura=atof(datos[4]);
                tiempo=atof(datos[6]);
                tiempo_div=tiempo/1000;
                ts=*localtime(&tiempo_div);
                strftime(buf,sizeof (buf),"%a %Y-%m-%d %H:%M:%S %Z",&ts);
                ui->datos->insertRow(ui->datos->rowCount());
                ui->datos->setItem(ui->datos->rowCount()-1,0,new QTableWidgetItem(datos[1]));
                ui->datos->setItem(ui->datos->rowCount()-1,1,new QTableWidgetItem(datos[2]));
                ui->datos->setItem(ui->datos->rowCount()-1,2,new QTableWidgetItem(datos[3]));
                ui->datos->setItem(ui->datos->rowCount()-1,3,new QTableWidgetItem(datos[4]));
                ui->datos->setItem(ui->datos->rowCount()-1,4,new QTableWidgetItem(buf));
                if(velocidad>1800-Tv && velocidad<1800+Tv){
                    ui->error_vel->setVisible(false);
                    ui->verif_vel->setVisible(true);
                    ui->verif_vel->setText("El valor de la velocidad se encuentra dentro\n del rango");

                }
                else{
                    ui->verif_vel->setVisible(false);
                    ui->error_vel->setVisible(true);
                    ui->error_vel->setText("El valor de la velocidad se encuentra fuera\n del rango, por favor corregirlo");

                }
                if(frecuencia>60-(60*0.05) && frecuencia<60+(60*0.05)){
                    ui->error_frec->setVisible(false);
                    ui->verif_frec->setVisible(true);
                    ui->verif_frec->setText("El valor de la frecuencia se encuentra dentro\n del rango");

                }
                else{
                    ui->verif_frec->setVisible(false);
                    ui->error_frec->setVisible(true);
                    ui->error_frec->setText("El valor de la frecuencia se encuentra fuera\n del rango, por favor corregirlo");

                }
                if(voltaje>120-Tvolt && voltaje<120+Tvolt){
                    ui->error_volt->setVisible(false);
                    ui->verif_volt->setVisible(true);
                    ui->verif_volt->setText("El valor del voltaje se encuentra dentro\n del rango");

                }
                else{
                    ui->verif_volt->setVisible(false);
                    ui->error_volt->setVisible(true);
                    ui->error_volt->setText("El valor del voltaje se encuentra fuera\n del rango, por favor corregirlo");

                }
                if(temperatura>40-Tt && temperatura<40+Tt){
                    ui->error_temp->setVisible(false);
                    ui->verif_temp->setVisible(true);
                    ui->verif_temp->setText("El valor de la temperatura se encuentra dentro\n del rango");

                }
                else{
                    ui->verif_temp->setVisible(false);
                    ui->error_temp->setVisible(true);
                    ui->error_temp->setText("El valor de la temperatura se encuentra fuera\n del rango, por favor corregirlo");

                }
              }
            else
                ui->error->setText("\tUsuario no encontrado");

        }
    }
    else
        ui->error->setText("La direccion IPv4 o el puerto que ha\n\n\t ingresado son incorrectos");
}

