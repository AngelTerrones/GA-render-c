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
 * \class Cromosoma
 * \version 0.8
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para la codificacion de los parametros que diferencian a cada
 * individuo, y que constituyen soluciones a un problema.
 */

#include "Cromosoma.h"

/******************************************************************************
Constructor
******************************************************************************/
/**
 * \fn Cromosoma::Cromosoma()
 * Constructor por defecto de la clase.
 * Inicializa los apuntadores a NULL, y otras variables a cero
 */
Cromosoma::Cromosoma()
{
    longitudCromosoma = 0;
    listaPoligono = NULL;
    uLLfitness = 0;
}

/**
 * \fn Cromosoma::~Cromosoma()
 * Destructor de la clase. Libera la memoria asignada dinamicamente.
 */
Cromosoma::~Cromosoma()
{
    Borrar(); // eliminar la memoria asignada en forma dinámica
}


/**
 * \fn void Cromosoma::Crear(int)
 * Asigna espacio de memoria de una determinada longitud al cromosoma.
 *
 * @param nPoligonos Longitud del cromosoma.
 */
void Cromosoma::Crear(int nPoligonos)
{
    Borrar(); // eliminar la memoria asignada previamente (por si acaso)

    listaPoligono = new Poligono[nPoligonos]; // crear la lista de apuntadores

    longitudCromosoma = nPoligonos;
    uLLfitness = 0;
}

/**
 * \fn void Cromosoma::Borrar()
 * Elimina la informacion en el cromosoma y libera la memoria
 */
void Cromosoma::Borrar()
{
    if(listaPoligono != NULL)
        delete[] listaPoligono; // [] ya que estoy creando un array

    listaPoligono = NULL;
    uLLfitness = 0; //fitness igualo a cero
    longitudCromosoma = 0; //hago tamaño igual a cero
}

/**
 * \fn unsigned int Cromosoma::LongitudCromosoma()
 * Indica la longitud del cromosoma
 * @return La longitud del cromosoma
 */
int Cromosoma::LongitudCromosoma()
{
    return longitudCromosoma;
}

/**
 * \fn void Cromosoma::Clonar(Cromosoma *)
 * Copia la informacion contenida en esta instancia a otra.
 * @param copia Apuntador al cromosoma destino donde se copiaran los datos.
 */
void Cromosoma::Clonar(Cromosoma *copia)
{
    if(!longitudCromosoma)
        return; // no tengo datos para copiar

    copia->Crear(longitudCromosoma); // crear
    copia->uLLfitness = uLLfitness;  // copiar el fitness

    // copiar los datos del cromosoma
    for(int i = 0; i < longitudCromosoma; i++)
        copia->listaPoligono[i] = listaPoligono[i]; // usando sobrecarga de operadores (ahora si)!
}

/**
 * \fn void Cromosoma::Mutar(double)
 * Operador de mutacion sobre el cromosoma
 *
 * @param tasa Tasa a la que se mutara el cromosoma
 */
void Cromosoma::Mutar()
{
    // añadir un punto en un lugar aleatorio para cada individuo
    for(int i = 0; i < longitudCromosoma; i++)
    {
        int nVertices = listaPoligono[i].Npuntos(); // obtener cuantos vertices tiene este polígono
        if(nVertices < MAXPUNTOS && ((rand() % 100) < 10)) // si no me paso del número máximo de vertices, con tasa de un 10% añadir un punto
        {
            int posicion = rand()%(nVertices-1) + 1;    // obtener posición de inserción [1 nVertices-1]
            QPoint nuevoVertice = (listaPoligono[i].Vertices()[posicion - 1] + listaPoligono[i].Vertices()[posicion])/2; // crear el punto en el medio
            listaPoligono[i].Vertices().insert(posicion, nuevoVertice);      // insertar
            listaPoligono[i].Npuntos(++nVertices);                    // incrementar la cuenta;
        }

        if(nVertices > MINPUNTOS && ((rand() % 100) < 10)) // si no me paso del número mínimo de vertices, y con tasa de 10%, eliminar un punto
        {
            int posicion = rand()%(nVertices);    // obtener posición del vertice a eliminar [0 nVertices-1]
            listaPoligono[i].Vertices().removeAt(posicion); // eliminar
            listaPoligono[i].Npuntos(--nVertices);                    // incrementar la cuenta;
        }
    }

    // mutar un solo campo a la vez. no todo de un solo golpe (solo por prueba). Elegir aleatoriamente
    int poligono = rand()%longitudCromosoma;                        // elegir aleatoriamente cual polígono hay que mutar
    int campo = rand()%(4 + listaPoligono[poligono].Npuntos());     // numero total de campos
    // Modificar el campo seleccionado en forma aleatoria
    switch(campo)
    {
        case 0:
            listaPoligono[poligono].Red(listaPoligono[poligono].Red() + rand()%51 - 25);
            break;
        case 1:
            listaPoligono[poligono].Green(listaPoligono[poligono].Green() + rand()%51 - 25);
            break;
        case 2:
            listaPoligono[poligono].Blue(listaPoligono[poligono].Blue() + rand()%51 - 25);
            break;
        case 3:
            listaPoligono[poligono].Alpha(listaPoligono[poligono].Alpha() + rand()%51 - 25);
            break;
        default:
            if(rand()%2) // determinar cual de las dos coordenadas mutar
            {
                int temp = listaPoligono[poligono].Vertices()[campo-4].x() + rand()%21 - 10;
                listaPoligono[poligono].Vertices()[campo-4].rx() = max(0, min(temp, listaPoligono[poligono].yMax));
            }
            else
            {
                int temp = listaPoligono[poligono].Vertices()[campo-4].y() + rand()%21 - 10;
                listaPoligono[poligono].Vertices()[campo-4].ry() = max(0, min(temp, listaPoligono[poligono].xMax));
            }
            break;
    }
}
/***************************************************************************
Fin del archivo
***************************************************************************/
