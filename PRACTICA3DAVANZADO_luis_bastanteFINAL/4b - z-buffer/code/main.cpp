/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Autor: Luis Bastante Fraile                                                *
*                                                                             *
*  Proyecto: Practica 3D avanzado version 1.0.                                *
*                                                                             *
*  Fecha: 27/01/2018                                                          *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//CLASE MAIN
//funcion que se encarga de crear el viewport y realiza la ejecucion del programa.

#include "View.hpp"
#include <list>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace example;

static const size_t window_width  = 800;
static const size_t window_height = 600;

int main ()
{
    // Creamos la ventana
	Window window(VideoMode(window_width, window_height), "Z-Buffer", Style::Titlebar | Style::Close, ContextSettings(32));

	// Creamos el viewPort con el ancho y el alto deseados
    View view  (window_width, window_height);

    window.setVerticalSyncEnabled (true);

    glDisable (GL_BLEND);
    glDisable (GL_DITHER);
    glDisable (GL_CULL_FACE);
    glDisable (GL_DEPTH_TEST);
    glDisable (GL_TEXTURE_2D);

    glViewport     (0, 0, window_width, window_height);
    glMatrixMode   (GL_PROJECTION);
    glLoadIdentity ();
    glOrtho        (0, GLdouble(window_width), 0, GLdouble(window_height), -1, 1);

	//iniciamos el bucle de ejecucion 
    bool running = true;

    do
    {
        //detectamos con eventos si la ventana se ha cerrado para detener la ejecucion
        Event event;

        while (window.pollEvent (event))
        {
            if (event.type == Event::Closed)
            {
                running = false;
            }
        }

        //llamamos al update de view
        view.update ();

        //llamamos al metodo paint de view
        view.paint ();

        window.display ();
    }
    while (running);

    //Cerrar aplicacion
    return (EXIT_SUCCESS);
}
