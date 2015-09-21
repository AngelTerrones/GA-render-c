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

#ifndef __MAINWINDOWAGRENDERER_H
#define __MAINWINDOWAGRENDERER_H

#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QFileDialog>
#include <QPushButton>
#include <QSvgGenerator>
#include "threadAG.h"
#include "imagenWidget.h"
#include "renderWidget.h"
#include "dlgConfigurarAG.h"
#include "ui_MainWindowAGRenderer.h"

class MainWindowAGRenderer : public QMainWindow,
                             private Ui::MainWindowAGRenderer
{
    Q_OBJECT
    public:
        MainWindowAGRenderer(); // Constructor del panel frontal del programa
    protected:
        void closeEvent(QCloseEvent *event); // Evento de finalización
    private slots:
        void AcercaRenderizadorGenetico();
        void AbrirImagen();
        void GuardarImagen();
        void ConfigurarAG();
        void IndicarParametros(int npob, double mutacion, int npolig,
                               int npuntos);
        void IniciarAGRenderer();
        void DetenerAGRenderer();
        void ActualizarCanvas(QImage imagen, int generacion,
                              unsigned long long fitness);
    private:
        // funciones
        void InicializarCanvas(const QString nombre_imagen);
        // variables
        imagenWidget *Imagen;   // widget donde se muestra la img original
        renderWidget *ImagenEvolutiva; // widget para muestra la img generada
        QImage ImagenOriginal; // imagen original
        dlgConfigurarAG *ptDlgConfigurarAG; // dialogo de configuracion
        QPushButton *buttonIniciarAG; // boton de inicio
        QSize sizeBotton; // tamanho del boton... ¬¬

        QLabel *StatusBar;

        threadAG *thAG;
        bool renderizando;
        int anchoImagen;
        int altoImagen;

        // parametros AG
        int npoblacion;
        double tasa_mutacion;
        int num_poligonos;
        int num_puntos;
};
#endif
/***************************************************************************
Fin del archivo
***************************************************************************/
