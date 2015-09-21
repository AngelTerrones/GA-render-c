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
 * \class renderWidget 
 * \version 0.1
 * \author Angel Terrones
 * \author Dimitar Ralev
 *
 * Clase para mostrar la imagen renderizada
 */
 
#include "renderWidget.h"

/******************************************************************************
Constructor
******************************************************************************/
/**
 * \fn renderWidget::renderWidget (QWidget *, const QSize)
 * Constructor por defecto de la clase.
 * Inicializa el Widget
 * @param parent Padre del Widget.
 * @param tamanho Dimensiones de la imagen original.
 */
renderWidget::renderWidget (QWidget *parent, const QSize tamanho)
             :QWidget(parent)
{
    Imagen = QPixmap(tamanho);
    QColor color = Qt::black;
    Imagen.fill(color);
}
 
/**
 * \fn QPixmap renderWidget::ImagenRenderizada()
 * Devuelve la imagen que muestra en el widget.
 * @return Imagen renderizada en el Widget
 */
QPixmap renderWidget::ImagenRenderizada()
{
    return Imagen;
}

/**
 * \fn void renderWidget::ActualizarImagen(QImage)
 * Convierte la imagen generada por el AG en formato QImage a mapa de 
 * pixeles para ser mostrada en el Widget.
 * @param imagen Imagen generada por el AG.
 */
void renderWidget::ActualizarImagen(QImage imagen)
{
    Imagen = QPixmap::fromImage(imagen);
}
/******************************************************************************
Manejo de Eventos
******************************************************************************/
/**
 * \fn void renderWidget::paintEvent(QPaintEvent* event)
 * Reimplementacion del evento de actualizacion del Widget, para mostrar la
 * imagen generada por el AG
 *
 * @param event Apuntador al evento de Actualizacion
 */
void renderWidget ::paintEvent(QPaintEvent* event)
{    
    QWidget::paintEvent(event);
    QPainter render(this);
    render.setRenderHint(QPainter::Antialiasing, true);
    render.drawPixmap(0, 0, Imagen);
    render.end();    
}
/***************************************************************************
Fin del archivo
***************************************************************************/