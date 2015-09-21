/***************************************************************************
 *   Copyright (C) 2010 by Angel Terrones                                  *
 *   Copyright (C) 2010 by Dimitar Ralev                                   *
 *   angelterrones@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/** 
 * \class MainWindowAGRenderer
 * \version 0.8
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para la creacion de la GUI del programa, y manejo del renderizado
 * de la imagen original y renderizada en ventana.
 * Esta clase contiene los métodos referentes a la creación y manejo
 * de la ventana principal.
 */
 
#include "MainWindowAGRenderer.h"

/******************************************************************************
Constructor
******************************************************************************/
/**
 * \fn MainWindowAGRenderer::MainWindowAGRenderer()
 * Contructor por defecto de la clase.
 */
MainWindowAGRenderer::MainWindowAGRenderer()
                     :QMainWindow(0)
{
    setupUi(this); // Inicializa instancia de la ventana usando el archivo Ui
    connect(action_Salir, SIGNAL(triggered()), this, SLOT(close()));
    connect(action_Cargar_Imagen, SIGNAL(triggered()), this, 
            SLOT(AbrirImagen()));
    connect(action_Guardar_Imagen, SIGNAL(triggered()), this, 
            SLOT(GuardarImagen()));
    connect(action_Acerca_de_Renderizador_Genetico, SIGNAL(triggered()), this,
            SLOT(AcercaRenderizadorGenetico()));
    connect(action_Configurar, SIGNAL(triggered()), this, 
            SLOT(ConfigurarAG()));
    
    // iniciliza los vectores a NULL (algo que no hace el cappibbarra... XD)
    thAG = NULL;
    Imagen = NULL;
    ImagenEvolutiva = NULL;
    buttonIniciarAG = NULL;
    ptDlgConfigurarAG = NULL;
    InicializarCanvas(":img/imagenes/ml.bmp"); // usar imagen en el qrc. No funciona en la maquina de la universidad...
    
    renderizando = FALSE; // variable interna. STFO
    
    // Parametros por defecto
    num_poligonos = MAXPOLIGONOS;
    num_puntos= MINPUNTOS;
    npoblacion = MAX_POBLACION;
    tasa_mutacion = 0.1; 
    
    // barra de status para informar al usuario.
    StatusBar = new QLabel(QString("Fitness ") + QString::number(0) + 
                           QString("\tGeneracion ") + QString::number(0) + 
                           QString("\tPoblacion ") + 
                           QString::number(npoblacion) +
                           QString("\tPoligonos ") + 
                           QString::number(num_poligonos) );
    statusbar->addWidget(StatusBar);
}

/******************************************************************************
Manejo de Eventos
******************************************************************************/
/**
 * \fn void MainWindowAGRenderer::closeEvent(QCloseEvent *)
 * Callback al evento de finalizar programa. 
 * Finaliza la ejecucion y limpia variables antes de finalizar el programa.
 * @param event Apuntador al evento Close
 */
void MainWindowAGRenderer::closeEvent(QCloseEvent *event)
{    
    DetenerAGRenderer();
    // limpiar la memoria
    if(thAG != NULL)
    {
        //thAG->~threadAG();
        delete thAG;
    }
    delete Imagen;
    delete ImagenEvolutiva;
    delete buttonIniciarAG;
    event->accept();   // aceptar el evento y salir
}

/******************************************************************************
Implementacion de los SLOTs
******************************************************************************/
/**
 * \fn void MainWindowAGRenderer::AcercaRenderizadorGenetico()
 * Muestra ventana con los autores del programa :D.
 */
void MainWindowAGRenderer::AcercaRenderizadorGenetico()
{
    QMessageBox::about(this, "Acerca de Renderizador Genetico",
                       QString::fromUtf8("<h2>Renderizador Genetico</h2>"
                       "<p>Copyright &copy; 2010 Angel Terrones<p>"
                       "<p>Copyright &copy; 2010 Dimitar Ralev<p>"));
}

/**
 * \fn void MainWindowAGRenderer::AbrirImagen()
 * Mustra una ventana para seleccionar el archivo de imagen que se desea
 * generar geneticamente.
 */
void MainWindowAGRenderer::AbrirImagen()
{
    if(!renderizando) // solo si no estoy generando imagenes
    {
        QString temp = QFileDialog::getOpenFileName(this, "Abrir imagen", "", 
                                                    "Images (*.png *.bmp *.jpg)");
        if(temp.isEmpty())
            return; 
        // cargar la nueva imagen. Reiniciar todo
        InicializarCanvas(temp); 
    }
}


/**
 * \fn void MainWindowAGRenderer::GuardarImagen()
 * Muestra una ventana para indicar donde se exportara el archivo generado 
 * dinamicamente por el programa.
 * El archivo se exporta en formato SVG.
 */
void MainWindowAGRenderer::GuardarImagen()
{
    if(!renderizando) // solo si no estoy generando imagenes
    {
        QString temp = QFileDialog::getSaveFileName(this, 
                                                "Guardar Imagen Renderizada",
                                                "untitled.svg", "Documento SVG "
                                                "(*.svg)");
        if(temp.isEmpty())
            return;
        
        //Guardar la imagen renderizada en svg. NPI de porque
        // la escala a 1.25 veces el tamaño o.O
        QSvgGenerator generador;
        generador.setFileName(temp);
        generador.setSize(QSize(anchoImagen*ZOOM, altoImagen*ZOOM));
        generador.setViewBox(QRect(0, 0, anchoImagen*ZOOM, altoImagen*ZOOM));
        QPainter painter(&generador);
        painter.setPen(Qt::NoPen);
        painter.drawPixmap(0, 0, ImagenEvolutiva->ImagenRenderizada());
        painter.end();
    }
}

/**
 * \fn void MainWindowAGRenderer::ConfigurarAG()
 * Muestra un dialogo de configuracion de los parametros del AG
 */
void MainWindowAGRenderer::ConfigurarAG()
{
    if(!renderizando)
    {
        ptDlgConfigurarAG = new dlgConfigurarAG(this);
        ptDlgConfigurarAG->ParametrosDefault(npoblacion, tasa_mutacion, 
                                             num_poligonos, num_puntos);
        connect(ptDlgConfigurarAG, 
                SIGNAL(IndicarParametros(int, double, int, int)),
                this, SLOT(IndicarParametros(int, double, int, int)));
        ptDlgConfigurarAG->exec();
        ptDlgConfigurarAG->~dlgConfigurarAG(); 
    }
}

/**
 * \fn void MainWindowAGRenderer::IndicarParametros(int, double, int, int)
 * Indica los nuevos parametros del AG, obtenidos del dialogo de configuracion
 *
 * @param npob Numero de individuos de la poblacion
 * @param mutacion Tasa de mutacion
 * @param npolig Numero de poligonos a utilizar
 * @param npuntos Numero de puntos por poligono
 */
void MainWindowAGRenderer::IndicarParametros(int npob, double mutacion, int npolig, int npuntos)
{
    npoblacion = npob;
    tasa_mutacion = mutacion;
    num_poligonos = npolig;
    num_puntos = npuntos;
}

/**
 * \fn void MainWindowAGRenderer::IniciarAGRenderer()
 * Inicializa el thread donde se ejecutara el AG.
 */
void MainWindowAGRenderer::IniciarAGRenderer()
{
    if(thAG == NULL) // si no existe una instancia, crearla
    {
        thAG = new threadAG(this, ImagenOriginal);
        if(thAG == NULL)
        {
            qDebug("ERROR en thAG");            
            return;
        }
        thAG->configurarAG(npoblacion, tasa_mutacion, num_poligonos, num_puntos);
        
        connect(thAG, SIGNAL(ActualizarCanvas(QImage, int, unsigned long long)), this, 
                SLOT(ActualizarCanvas(QImage, int, unsigned long long)));
    }
    thAG->start(QThread::HighestPriority);
    
    if(thAG->isRunning()) // verificar si se esta ejecutando
    {
        disconnect(buttonIniciarAG, SIGNAL(clicked()), this, 
                   SLOT(IniciarAGRenderer()));
        
        menu_Imagen->setDisabled(TRUE);
        menu_ADN->setDisabled(TRUE);
        action_Configurar->setDisabled(TRUE);
        
        connect(buttonIniciarAG, SIGNAL(clicked()), this, 
                SLOT(DetenerAGRenderer()));
        buttonIniciarAG->setText("Detener"); 
        renderizando = TRUE;
    }
    else
    {
        delete thAG;
        QMessageBox::critical(this, "ERROR\n",
                              QString::fromUtf8("No se pudo iniciar Thread"),
                              QMessageBox::Yes, QMessageBox::Yes);
    }
}


/**
 * \fn void MainWindowAGRenderer::DetenerAGRenderer()
 * Finaliza la ejecucion del thread que realiza el AG.
 */
void MainWindowAGRenderer::DetenerAGRenderer()
{
    if(thAG != NULL && thAG->isRunning())
        thAG->stop();
        
    disconnect(buttonIniciarAG, SIGNAL(clicked()), this, 
               SLOT(DetenerAGRenderer()));
    menu_Imagen->setDisabled(FALSE);
    menu_ADN->setDisabled(FALSE);
    action_Configurar->setDisabled(FALSE);
    connect(buttonIniciarAG, SIGNAL(clicked()), this, 
            SLOT(IniciarAGRenderer()));
    buttonIniciarAG->setText("Iniciar");
    renderizando = FALSE;
}

/**
 * \fn void MainWindowAGRenderer::ActualizarCanvas(QImage, int, unsigned long long)
 * Actualiza el Widget para mostrar la imagen que genera el AG.
 *
 * @param imagen Imagen generada por el AG
 * @param generacion Numero de generacion de la poblacion
 * @param fitness Fitness del mejor de la poblacion
 */
void MainWindowAGRenderer::ActualizarCanvas(QImage imagen, int generacion, 
                                            unsigned long long fitness)
{
    if(generacion%10 == 0)
    {
        delete StatusBar;
        StatusBar = new QLabel(QString("Fitness = ") + 
                               QString::number(fitness) +
                               QString(" | Generacion = ") + 
                               QString::number(generacion) +
                               QString(" | Poblacion = ") + 
                               QString::number(npoblacion) +
                               QString(" | Poligonos = ") + 
                               QString::number(num_poligonos) );
        statusbar->addWidget(StatusBar);
    }
    ImagenEvolutiva->ActualizarImagen(imagen);
    ImagenEvolutiva->show();
    ImagenEvolutiva->update(); //refresh
}

/******************************************************************************
Implementación de las funciones Privadas
******************************************************************************/
/**
 * \fn void MainWindowAGRenderer::InicializarCanvas(const QString nombre_imagen)
 * Inicializa los Widgets donde se muestran la imagen original y la generada
 * @param nombre_imagen Ruta de la imagen a renderizar.
 */
void MainWindowAGRenderer::InicializarCanvas(const QString nombre_imagen)
{
    if(buttonIniciarAG == NULL) // crear el boton una sola vez...
    {
        buttonIniciarAG = new QPushButton("Iniciar",this);
        connect(buttonIniciarAG, SIGNAL(clicked()), this, 
                SLOT(IniciarAGRenderer()));
        sizeBotton = buttonIniciarAG->size(); // obtener el tamanho por defecto
        //buttonIniciarAG->resize(sizeBotton + QSize(50,0)); // incrementar el ancho
    }    
    
    ImagenOriginal = QImage(nombre_imagen); // cargar imagen
    anchoImagen = ImagenOriginal.width(); // obtener dimensiones
    altoImagen = ImagenOriginal.height();
    // 20 menu, 10 espacio, 22 status bar
    QSize tamanhoVentana((anchoImagen*(1+ZOOM))+30, ZOOM*altoImagen + 20 + 10 + 22 ); // calcular tamanho
    //this->resize(tamanhoVentana); // cambiar tamaño de la ventana
    this->setFixedSize(tamanhoVentana);

    // mover el boton
    buttonIniciarAG->move(10, 30 + ZOOM*altoImagen - buttonIniciarAG->size().height());
    
    // rehacer la imagen original
    if(Imagen != NULL)  // borrar instancia anterior, si existe
        delete Imagen;
    Imagen = new imagenWidget(centralwidget, ImagenOriginal); // crear widget
    Imagen->move(10,10);   // mover al margen
    QSize tamanhoWidgetImagen(anchoImagen, altoImagen);
    Imagen->resize(tamanhoWidgetImagen);   // modificar tamanho
    Imagen->show();
    Imagen->update(); //refresh
    
    // rehacer el canvas
    if(ImagenEvolutiva != NULL)
        delete ImagenEvolutiva;    
    ImagenEvolutiva = new renderWidget(centralwidget, ZOOM*ImagenOriginal.size());
    ImagenEvolutiva->move(20+anchoImagen,10);
    ImagenEvolutiva->resize(tamanhoWidgetImagen*ZOOM);
    ImagenEvolutiva->show();
    ImagenEvolutiva->update(); //refresh

    

    // limpiar apuntador
    if(thAG != NULL)
    {
        delete thAG;
        thAG = NULL; // reset del apuntador
        qDebug("thread borrado");
    }
}
/***************************************************************************
Fin del archivo
***************************************************************************/