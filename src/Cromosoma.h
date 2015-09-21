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

#ifndef __CROMOSOMA_H
#define __CROMOSOMA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <QDebug>
#include <QList>
#include <QPoint>
#include <algorithm>

#define MAXPUNTOS 40
#define MINPUNTOS 3
#define MAXPOLIGONOS 50

using namespace std;

// clase poligono. Para una mejor implementacion del perol este.
class Poligono
{
    public:
        Poligono(){                 // constructor
            r = rand()%255;         // valores aleatorios de color
            g = rand()%255;
            b = rand()%255;
            a = rand()%255;
            nPuntos = MINPUNTOS;   // asignar número mínimo de puntos (triangulo) y así evitar que se creen polígonos basura
            xMax = yMax = 0;       // límite por defecto

            for(int i = 0; i < nPuntos; i++)        // inicializar la lista (ya que esta empieza de tamaño cero)
                vertices << QPoint(0, 0);           // con un punto en cero
        }

        void init(int xMax, int yMax)   // inicializar las dimensiones a valores válidos
        {
            this->xMax = xMax;
            this->yMax = yMax;

            for(int i = 0; i < nPuntos; i++)
            {
                vertices[i].rx() += rand()%xMax; // ladilla de notación
                vertices[i].ry() += rand()%yMax;
            }
        }

        unsigned char Red(){return r;}              // obtener los valores
        unsigned char Green(){return g;}
        unsigned char Blue(){return b;}
        unsigned char Alpha(){return a;}
        int Npuntos(){return nPuntos;}
        QList<QPoint> &Vertices(){return vertices;}

        void Red(unsigned char color){ r = max((unsigned char)0, min(color, (unsigned char)255));}         // Inicializar los valores
        void Green(unsigned char color){ g = max((unsigned char)0, min(color, (unsigned char)255));}
        void Blue(unsigned char color){ b = max((unsigned char)0, min(color, (unsigned char)255));}
        void Alpha(unsigned char color){ a = max((unsigned char)0, min(color, (unsigned char)255));}
        void Npuntos(int puntos){ nPuntos = puntos;}

        // que vaina fumada es esta? o.O
        // tengo pleno acceso a los datos privados de cada uno de las clases!?
        Poligono& operator =(const Poligono &A)
        {
            if(this != &A) // evitar copiarse a si mismo
            {
                // proceder a la copia de los datos
                r = A.r;
                g = A.g;
                b = A.b;
                a = A.a;

                xMax = A.xMax;
                yMax = A.yMax;

                nPuntos = A.nPuntos;
                vertices = A.vertices;
            }

            return *this;
        }

        // eventualmente, mover esto a miembro privado
        int xMax, yMax;
    private:
        unsigned char r, g, b, a; //parámetros del color
        int nPuntos;
        QList<QPoint> vertices;
};

// clase para el manejo del DNA
class Cromosoma
{
    public:
                            Cromosoma();
                            ~Cromosoma();
        void                Crear(int nPoligonos);
        void                Borrar();
        int                 LongitudCromosoma();
        void                Clonar(Cromosoma *copia);
        void                Mutar();

        // variables
        Poligono *          listaPoligono;
        unsigned long long  uLLfitness;
    private:
        int                 longitudCromosoma;
};

#endif
/***************************************************************************
Fin del archivo
***************************************************************************/
