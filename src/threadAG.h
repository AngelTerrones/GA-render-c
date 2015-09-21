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

#ifndef __THREADAG_H
#define __THREADAG_H

#include <QThread>
#include <QPainter>
#include "Cromosoma.h"

#define MAX_POBLACION 10
#define FRACCION_POB 0.4
#define ZOOM 1.5

class threadAG : public QThread
{
    Q_OBJECT
    public:
        threadAG(QObject *parent, QImage ImagenO);
        ~threadAG();
        void stop();
        void configurarAG(int poblacion, double tasa_mut, int npoligonos, int npuntos);
    signals:
        void ActualizarCanvas(QImage imagen, int generacion, unsigned long long fitness);
    protected:
        void run();
    private:
        // variables
        bool bEjecutar;
        QImage ImagenOriginal;
        int anchoImagen;
        int altoImagen;
        // variables del render/AG (no se si vale la pena...)
        int npoblacion;
        double tasa_mutacion;
        int num_poligonos;
        int num_puntos;

        unsigned int ngeneraciones;
        /*
            vector de poblacion.
            Cada elemento apunta a un arreglo de poligonos, que a su vez
            contienen la informacion de aRGB y vertices del poligono.
            Eso constituye el cromosoma.
            El valor aRGB se codifica en double. Para más información, revisar la clase Polígono y Cromosoma
        */
        Cromosoma *poblacionA[MAX_POBLACION]; // Cambiar a memoria dinámica.
        Cromosoma *poblacionB[MAX_POBLACION];
        int iTamanoPoblacionA;
        int iTamanoPoblacionB;

        // funciones
        void InicializarPoblacion(int poblacion, int longitud);
        void runAG();
        void Generacion();
        Cromosoma *SeleccionarMejor();
        void InsertarPoblacionB(Cromosoma *cromosoma);
        Cromosoma *SeleccionarTournament();
        void Cruzar(Cromosoma *a, Cromosoma *b);
        int PoblacionLibre();
        void ActualizarPoblacion();

        unsigned long long fitness(Cromosoma *cromosoma);
        QImage RenderizarImagen(Cromosoma *cromosoma, bool zoom);

};

#endif
/***************************************************************************
Fin del archivo
***************************************************************************/
