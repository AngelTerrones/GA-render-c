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

#include <QApplication>
#include <QSplashScreen>
#include <time.h>
#include "MainWindowAGRenderer.h"

int main(int argc, char *argv[])
{
    int r = 0;
    #ifdef WIN32
    r = system("cls");      // Windows. Por si ejecuta en consola...
    #else
    r = system("clear");    // Linux
    #endif

    srand(time(NULL));   // inicializar generador aleatorio

    QApplication app(argc, argv);  // Crear instancia de una aplicacion gráfica
    MainWindowAGRenderer Ventana; // Crear una instancia de la Ventana Principal
    Ventana.show();           // Mostrar la Ventana
    return app.exec();        // Entrar al lazo infinito de ejecución
}
/***************************************************************************
Fin del archivo
***************************************************************************/
