/***************************************************************************
 *   Copyright (C) 2008 by Angel Terrones                                  *
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
 * \class dlgConfigurarAG
 * \version 0.9
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para mostrar una ventana de dialogo para los parametros del AG
 */

#include "dlgConfigurarAG.h"

/**
 * \fn dlgConfigurarAG::dlgConfigurarAG(QWidget *)
 * Constructor de la clase.
 * @param padre Padre de la instancia
 */
dlgConfigurarAG::dlgConfigurarAG(QWidget *padre)
                :QDialog(padre)
{
    setupUi(this);           // Configurar según la plantilla de diseño
    // Conectar las señales con los slots
    connect(guardar_PushButton, SIGNAL(clicked()), this, 
            SLOT(GuardarConfiguracion()));
    connect(default_PushButton, SIGNAL(clicked()), this, 
            SLOT(RestaurarConfiguracion()));
}

/**
 * \fn void dlgConfigurarAG::ParametrosDefault(int, double, int, int)
 * Indica los parametros del AG para mostrarlos en el dialogo de configuracion
 *
 * @param npob Numero de individuos en la poblacion
 * @param mutacion Tasa de mutacion
 * @param npolig Numero de poligonos
 * @param npuntos Numero de puntos
 */
void dlgConfigurarAG::ParametrosDefault(int npob, double mutacion, int npolig, 
                                        int npuntos)
{
    npoblacion = npob;
    tasa_mutacion = mutacion;
    num_poligonos = npolig;
    num_puntos = npuntos;
    RestaurarConfiguracion();
}

/******************************************************************************
Implementacion de los SLOTs
******************************************************************************/
/**
 * \fn void dlgConfigurarAG::GuardarConfiguracion()
 * Guarda los parametros de configuracion del AG y finaliza la ejecucion de la
 * ventana de dialogo.
 */
void dlgConfigurarAG::GuardarConfiguracion()
{
    npoblacion = poblacionSpinBox->value();
    tasa_mutacion = mutacionDoubleSpinBox->value();
    num_poligonos = npoligonosSpinBox->value();
    num_puntos = npuntosSpinBox->value();
    emit IndicarParametros(npoblacion, tasa_mutacion, num_poligonos, num_puntos);
    accept();
}


/**
 * \fn void dlgConfigurarAG::RestaurarConfiguracion()
 * Muestra en el dialogo de configuracion los parametros actuales del AG
 * sin modificar, eliminando los cambios hechos por el usuario.
 */
void dlgConfigurarAG::RestaurarConfiguracion()
{
    poblacionSpinBox->setValue(npoblacion);
    mutacionDoubleSpinBox->setValue(tasa_mutacion);
    npoligonosSpinBox->setValue(num_poligonos);
    npuntosSpinBox->setValue(num_puntos);
}
/***************************************************************************
Fin del archivo
***************************************************************************/