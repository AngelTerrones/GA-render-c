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
 * \class imagenWidget
 * \version 0.9
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para mostrar la imagen que se desea renderizar
 */
 
#include "imagenWidget.h"

/******************************************************************************
Constructor
******************************************************************************/
/**
 * \fn imagenWidget::imagenWidget(QWidget *, const QImage &)
 * Contructor por defecto de la clase.
 * Inicializa las variables internas de la clase
 * @param parent Padre el Widget
 * @param imagen Imagen a mostrar
 */
imagenWidget::imagenWidget(QWidget *parent, const QImage &imagen)
             :QWidget(parent)
{
   Imagen = QPixmap::fromImage(imagen);
   ImagenOriginal = imagen;
}

/******************************************************************************
Manejo de Eventos
******************************************************************************/
/**
 * \fn void imagenWidget::paintEvent(QPaintEvent* event)
 * Reimplementacion del evento de actualizacion del Widget, para mostrar la
 * imagen a generar por el AG
 *
 * @param event Apuntador al evento de Actualizacion
 */
void imagenWidget::paintEvent(QPaintEvent* event)
{    
    QWidget::paintEvent(event);
    QPainter render(this);
    render.drawPixmap(0, 0, Imagen);    
}
/***************************************************************************
Fin del archivo
***************************************************************************/