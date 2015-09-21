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

#ifndef __DLGCONFIGURARAG_H
#define __DLGCONFIGURARAG_H

#include <QtGui>
#include "ui_dlgConfigurarAG.h"

class dlgConfigurarAG : public QDialog, private Ui::dlgConfigurarAG
{
    Q_OBJECT
    public:
        dlgConfigurarAG(QWidget *padre = 0);
        void ParametrosDefault(int npob, double mutacion, int npolig, 
                               int npuntos);
    signals:
        void IndicarParametros(int npob, double mutacion, int npolig, 
                               int npuntos);
    private slots:
        void GuardarConfiguracion();
        void RestaurarConfiguracion();
    private:
        int npoblacion;
        double tasa_mutacion;
        int num_poligonos;
        int num_puntos;
};
#endif
/***************************************************************************
Fin del archivo
***************************************************************************/