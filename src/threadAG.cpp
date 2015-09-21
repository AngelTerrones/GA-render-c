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
 * \class threadAG
 * \version 0.85
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para la generacion de un thread que ejecutara el AG para evitar 
 * que la ventana principal se ralentize.
 */
 
#include "threadAG.h"

/******************************************************************************
Constructor
******************************************************************************/
/**
 * \fn threadAG::threadAG(QObject *, QImage)
 * Constructor de la clase. Inicializa las poblaciones en NULL y las variables
 * del AG.
 * @param parent Padre del thread.
 * @param ImagenO Imagen a renderizar.
 */
threadAG::threadAG(QObject *parent, QImage ImagenO)
         :QThread(parent)
{
    bEjecutar = TRUE; // habilitar ejecucion
    ImagenOriginal = ImagenO; // Copiar el apuntador a la imagen original
    anchoImagen = ImagenOriginal.width(); // obtener dimensiones
    altoImagen = ImagenOriginal.height();
    
    // reset variables
    iTamanoPoblacionA = 0;
    iTamanoPoblacionB = 0;
    ngeneraciones = 0;
    tasa_mutacion = 0.001;
    
    for(int i = 0; i < MAX_POBLACION; i++)
    {
        poblacionA[i] = NULL;
        poblacionB[i] = NULL;
    }
    moveToThread(this);
}


/**
 * \fn threadAG::~threadAG()
 * Destructor de la clase. Hace algo que cappelletto no hace: 
 * LIMPIAR LA MEMORIA ¬¬
 */
threadAG::~threadAG()
{
    // poblacion A
    for(uint i = 0; i < MAX_POBLACION; i++)
    {
        if(poblacionA[i] != NULL)
            delete poblacionA[i];
    }
    
    // poblacion B
    for(uint i = 0; i < MAX_POBLACION; i++)
    {
        if(poblacionB[i] != NULL)
            delete poblacionB[i];
    }
}

/******************************************************************************
Implementación de las funciones publicas
******************************************************************************/
/**
 * \fn void threadAG::stop()
 * Finalizar la ejecucion del thread.
 */
void threadAG::stop()
{
    bEjecutar = FALSE; // inhabilitar ejecucion
    if(!wait(500))      // Esperar hasta que finalice la ejecución.
        terminate();    // timeout: abortar proceso
    bEjecutar = TRUE;  // volver a habilitar futuras ejecuciones
}

/**
 * \fn void threadAG::configurarAG(int, double, int, int)
 * Realiza la configuracion del AG, y llama a InicializarPoblacion para, 
 * valga la redundancia, incializar la poblacion A y la poblacion B.
 */
void threadAG::configurarAG(int poblacion, double tasa_mut, int npoligonos, int npuntos)
{
    tasa_mutacion = tasa_mut;           // tasa de mutación
    num_poligonos = npoligonos;         // número de polígonos para cada individuo de la población
    num_puntos = npuntos;               // interpretar como número máximo de puntos
    InicializarPoblacion(poblacion, npoligonos);    // inicializar
}


/******************************************************************************
Implementación de las funciones protected
******************************************************************************/
/**
 * \fn void threadAG::run()
 * Funcion que ejecuta el thread mientras la variable bEjecutar sea igual a
 * TRUE.
 */
void threadAG::run()
{
    while(bEjecutar)
        runAG();
}

/******************************************************************************
Implementación de las funciones Privadas
******************************************************************************/

/**
 * \fn void threadAG::InicializarPoblacion(int poblacion, int longitud)
 * Inicializar la poblacion A y B.
 *
 * @param poblacion Tamaño de la poblacion.
 * @param longitud Longitud del cromosoma
 */
void threadAG::InicializarPoblacion(int poblacion, int longitud)
{
    if (iTamanoPoblacionA != 0) //si la poblacion inicialmente es no nula 
    {
        for (int i=0; i < iTamanoPoblacionA; i++)
        {
            if(poblacionA[i] != NULL)
                delete poblacionA[i]; //libero cada uno de los individuos
        }
    }
    
    if (iTamanoPoblacionB != 0) //igual manera opero sobre la poblacion B
    {
        for (int i=0; i<iTamanoPoblacionB; i++)
        {
            if(poblacionB[i] != NULL)
                delete poblacionB[i]; //libero cada uno de los individuos
        }
    }
    iTamanoPoblacionB = iTamanoPoblacionA = 0; //esta todo vacio
    
    for (int i=0; i < poblacion; i++)
    {
        poblacionA[iTamanoPoblacionA] = new Cromosoma; // creo un nuevo cromosoma
        poblacionA[iTamanoPoblacionA]->Crear(longitud); //de longitud dada
        for(int j = 0; j < longitud; j++)
            poblacionA[iTamanoPoblacionA]->listaPoligono[j].init(anchoImagen, altoImagen);
        poblacionA[iTamanoPoblacionA]->uLLfitness = fitness(poblacionA[iTamanoPoblacionA]);
        iTamanoPoblacionA++; //incremento poblacion*/
    }
    ngeneraciones = 0;
}

/**
 * \fn void threadAG::runAG()
 * Funcion que ejecuta el AG, incrementa el numero de generaciones y emite la 
 * señal de actualizacion a la ventana principal para Actualizar el widget de 
 * la imagen renderizada.s
 */
void threadAG::runAG()
{
    ngeneraciones++;
    Generacion();   // crear una nueva generacion
    Cromosoma *cMejor = SeleccionarMejor();     // sacar el DNA del mejor individuo
    QImage mejor = RenderizarImagen(cMejor, true);    // Generar imagen
    emit ActualizarCanvas(mejor, ngeneraciones, cMejor->uLLfitness); // actualizar
}

/**
 * \fn void threadAG::Generacion()
 * Realiza el calculo de la proxima generacion. Nucleo del AG
 */
void threadAG::Generacion()
{
    Cromosoma *ap = NULL;
    Cromosoma *elite = NULL;
    Cromosoma clonA;
    Cromosoma clonB;
    
    // aplicar elitismo
    elite = SeleccionarMejor(); //obtengo apuntador al mejor de la poblacion
    elite->Clonar(&clonA); //saco una copia del mejor, para no afectar al original. Uso la funcion 'Clonar'
    InsertarPoblacionB(&clonA); //lo coloco directo en la poblacion B (poblacion nueva) sin modificar
    
    // harem
    int fraccion = (int)(FRACCION_POB * iTamanoPoblacionA);
    for (int i=0; i < fraccion;i++)
    {
        elite->Clonar(&clonA); //clonA <- copia(elite)
        SeleccionarTournament()->Clonar(&clonB); //clonB <- copia(otro) en forma aleatoria
        Cruzar(&clonA, &clonB); //lo cruzo en 'N' puntos de cruce y genereo DOS descendientes en A y B
        clonA.Mutar();  //muto cada uno de los descendientes
        clonB.Mutar();
        
        InsertarPoblacionB(&clonA); //los inserto en la poblacion nueva. Se generan 2*FRACCION_POB * iTamanoPoblacionA individuos de esta forma. No se verifica cual es el más apto
        InsertarPoblacionB(&clonA);
    }
    while (PoblacionLibre() > 0)  //mientras haya espacio libre para mas individuos...
    {
        //ahora procedo al proceso de seleccion segun criterio, por simplicidad esta implementacion usa seleccion tournament
        ap = SeleccionarTournament(); //veo ganador de seleccion por fitness,
        ap->Clonar(&clonA);
        ap = SeleccionarTournament(); //veo ganador de seleccion por fitness,
        ap->Clonar(&clonB);

        //ahora cruzo los dos clones
        Cruzar(&clonA, &clonB);

        // muto a los hijos
        clonA.Mutar();
        clonB.Mutar();
        
        // Luego inserto duplicado en la poblacion B, hasta donde quepa.
        // De los dos hijos, calculo el nuvo fitness y escojo el mejor
        clonA.uLLfitness = fitness(&clonA);
        clonB.uLLfitness = fitness(&clonB);
        if (clonA.uLLfitness < clonB.uLLfitness) // mejor fitness: el menor de los dos
            InsertarPoblacionB(&clonA);
        else    
            InsertarPoblacionB(&clonB);
    }
    ActualizarPoblacion(); // pasar B -> A
}

/**
 * \fn Cromosoma *threadAG::SeleccionarMejor()
 * Selecciona el individuo con mejor fitness en toda la poblacion
 *
 * @return Cromosoma del mejor individuo
 */
Cromosoma *threadAG::SeleccionarMejor ()
{
    if (!iTamanoPoblacionA) 
        return NULL;

    unsigned long long min = ~0x00; // interesante forma de obtener el maximo :D
    int cual = 0;
    
    for (int i=0; i<iTamanoPoblacionA; i++)
        if (poblacionA[i]->uLLfitness < min) //es mejor que el actual? 'cual'
        {
            min = poblacionA[i]->uLLfitness; //reasigno nuevo valor de maximo
            cual = i;
        }

    return poblacionA[cual];
}

/**
 * \fn void threadAG::InsertarPoblacionB(Cromosoma *)
 * Copia el cromosoma del individuo en la poblacion B
 *
 * @param cromosoma Cromosoma del individuo a insertar en la poblacion B
 */
void threadAG::InsertarPoblacionB(Cromosoma *cromosoma)
{
    if (iTamanoPoblacionA <= iTamanoPoblacionB) 
        return ; //no hay mas espacio
    
    if(poblacionB[iTamanoPoblacionB] == NULL)
        poblacionB[iTamanoPoblacionB] = new Cromosoma;

    cromosoma->Clonar(poblacionB[iTamanoPoblacionB]); //hago una copia exacta del individuo en la poblacion

    iTamanoPoblacionB++;
}

/**
 * \fn Cromosoma *threadAG::SeleccionarTournament()
 * Elige dos individuos aleatoriamente y selecciona el que tenga mejor fitness
 *
 * @return Cromosoma del individuo ganador.
 */
Cromosoma *threadAG::SeleccionarTournament()
{
    int i,j;
    i = rand()%iTamanoPoblacionA; //escojo dos individuos aleatoriamente con la misma probabilidad
    j = rand()%iTamanoPoblacionA;
    if (poblacionA[i]->uLLfitness < poblacionA[j]->uLLfitness) 
        return poblacionA[i];
    else 
        return poblacionA[j]; //devuelvo apuntador de mejor de los dos cromosomas evaluados
}

/**
 * \fn void threadAG::Cruzar(Cromosoma *, Cromosoma *)
 * Operador de cruce entre dos cromosomas.
 * @param a Cromosoma del individuo a
 * @param b Cromosoma del individuo b
 */
void threadAG::Cruzar(Cromosoma *a, Cromosoma *b)
{
    int longitud = a->LongitudCromosoma(); // asumo dos cromosomas de igual longitud. Número de polígonos a utilizar

    if (longitud <= 0) 
        return; //no hay nada que cruzar

    Cromosoma cA,cB;

    cA.Crear(longitud);
    cB.Crear(longitud); //reservo espacio

    for (int i=0; i < longitud; i++) //creo dos cromosomas nuevos
    {
        // elegir en forma aleatoria de quien hereda los datos del polígono completo
        if(rand()%2)
        {            
            cA.listaPoligono[i] = a->listaPoligono[i];
            cB.listaPoligono[i] = b->listaPoligono[i];
        }
        else
        {
            cA.listaPoligono[i] = b->listaPoligono[i];
            cB.listaPoligono[i] = a->listaPoligono[i];
        }
    }
    
    // modificar a y b
    // optimizado: dado que solo se mezclan entre si, mejor entonces hacer un swap en la operación anterior, y me ahorro la siguiente
    for(int i = 0; i < longitud; i++)
    {
        a->listaPoligono[i] = cA.listaPoligono[i];
        b->listaPoligono[i] = cB.listaPoligono[i];
    }

    cA.Borrar();
    cB.Borrar(); //libero espacio reservado por cromosomas temporales
}

/**
 * \fn int threadAG::PoblacionLibre()
 * Indica cuanto espacio queda en la poblacion B
 * @return Espacio restante en la poblacion B
 */
int threadAG::PoblacionLibre()
{
    return iTamanoPoblacionA - iTamanoPoblacionB;
}

/**
 * \fn void threadAG::ActualizarPoblacion()
 * Copia los individuos de la poblacion B a la poblacion A
 */
void threadAG::ActualizarPoblacion()
{
    for (int i=0; i<iTamanoPoblacionB; i++)
    {
        delete poblacionA[i]; 
        poblacionA[i] = new Cromosoma;
        poblacionA[i]->Crear(poblacionB[i]->LongitudCromosoma());
        
        poblacionB[i]->Clonar(poblacionA[i]); // copiar el cromosoma
    }

    // reemplazo el string del cromosoma, asi reduzco solicitud de espacio dinamicamente
    // a solo los casos donde se reinicia poblacion o se crea una poblacion nueva

    iTamanoPoblacionA = iTamanoPoblacionB; //todos
    iTamanoPoblacionB = 0; //nadie (deberia borrar la memoria?) Nop, de eso se encarga cada replica o el destructor
}
/**
 * \fn unsigned long long threadAG::fitness(Cromosoma *)
 * Funcion de costo del AG. Calcula la distancia al cuadrado ARGB de la imagen
 * renderizada a la imagen objetivo.
 * \code
   distancia <= a*a + R*R + G*G + B*B
 * \endcode
 *
 * @param cromosoma Cromosoma del individuo a evaluar
 * @return Fitness del individuo
 */
unsigned long long threadAG::fitness(Cromosoma *cromosoma)
{
    int nBytesImagen = ImagenOriginal.numBytes(); // obvio...
    QImage ImagenRender = RenderizarImagen(cromosoma, false);
    unsigned long long distancia = 0; // because long long long is too long for GCC... xD
    unsigned char *original = ImagenOriginal.bits(); // primer pixel
    unsigned char *render = ImagenRender.bits(); // primer pixel
    
    for(int i = 0; i < nBytesImagen; i++)
    {
        int temp = original[i] - render[i];
        distancia += temp*temp; // distancia al cuadrado...
    }
       
    return distancia;
}

/**
 * \fn QImage threadAG::RenderizarImagen(Cromosoma *)
 * Funcion que renderiza una imagen a partir del cromosoma de un individuo.
 *
 * La informacion del cromosoma se codifica se la siguiente manera
 * \code
 * [r,g,b,a,x0,y0,x1,y1,...,xN,yN]
 * \endcode
 * con lo que la longitud total del cromosoma es 
 * \code
 * longitud = 4 + 2*numero_de_puntos
 * \endcode
 * La codificacion de los parametros es la siguiente:
 * - alfa, R, G y B son de un byte de longitud.
 * - (xn,yn) se codifican en un byte.
 * La decodificacion se realiza de la siguiente forma:
 * - Los parametros ARGB son directos, dado que su rango es de 0 a 1.
 * - Las coordenadas se dividen entre 255, para obtener un valor de 0 a 1
 *   para obtener una coordenada relativa respecto a las dimensiones de
 *   la imagen objetivo.
 * 
 * @param cromosoma Cromosoma del individuo objetivo
 * @return Imagen renderizada
 */
QImage threadAG::RenderizarImagen(Cromosoma *DNA, bool zoom)
{
    double scale = (zoom? ZOOM: 1.0);
    QImage imagen(anchoImagen*scale, altoImagen*scale, QImage::Format_RGB32); //Format_ARGB32
    QPainter render(&imagen);
    render.setPen(Qt::NoPen); // NPI
    render.setRenderHint(QPainter::Antialiasing);
    
    // nikita :D
    render.setBrush(QBrush(QColor(0, 0 , 0, 255)));
    QPolygon polyclean;
    int pointsclean[] = {0, 0, 0, altoImagen*scale, anchoImagen*scale, altoImagen*scale, anchoImagen*scale, 0};
    polyclean.setPoints(4, pointsclean);
    render.drawPolygon(polyclean);        
    // nikita end
    
    Poligono *poli = DNA->listaPoligono;
    for (int i = 0; i < DNA->LongitudCromosoma(); i++) // num_poligonos es constante para cada individuo. Puede obtenerse del mismo DNA si es necesario
    {
        QColor color(poli[i].Red(), poli[i].Green(), poli[i].Blue(), poli[i].Alpha());
        render.setBrush(QBrush(QColor(color)));
        
        int size = poli[i].Npuntos();
        int *listaPuntos = new int[2*size];

        for(int j = 0; j < size; j++)
        {            
            listaPuntos[2*j] = scale*poli[i].Vertices()[j].x();
            listaPuntos[2*j + 1] = scale*poli[i].Vertices()[j].y();
        }

        QPolygon qpoli;
        qpoli.setPoints(size, listaPuntos);  
        render.drawConvexPolygon(qpoli);
        delete listaPuntos;
    }
    render.end();
    return imagen;
}
/***************************************************************************
Fin del archivo
***************************************************************************/