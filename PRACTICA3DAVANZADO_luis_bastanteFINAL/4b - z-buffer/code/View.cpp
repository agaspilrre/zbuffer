/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Autor: Luis Bastante Fraile                                                *
*                                                                             *
*  Proyecto: Practica 3D avanzado version 1.0.                                *
*                                                                             *
*  Fecha: 27/01/2018                                                          *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <cmath>
#include <cassert>
#include "View.hpp"
#include <Vector.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>

using namespace toolkit;

namespace example
{
	View::View(size_t width, size_t height)
		:
		root(new Mesh(pathfloor, 0.f, -1.298f,-10.0f, 0.15f)),
		width(width),
		height(height),
		Color_buffer(width, height),
		rasterizer(Color_buffer),
		projection(5, 15, 20, float(width) / float(height))
	{
		
		//inicializamos las variables de root, altura, anchura, buffers, rasterizer y projection
		//creamos los obj hijos
		std::shared_ptr< Mesh > mesh(new Mesh(pathHouse, -1.6f, 7.5f,-2.0f, 0.4f));
		std::shared_ptr<Mesh> mesh2(new Mesh(pathTree, 3.8f, 7.5f,-2.0f, 0.4f));
		
		//añadimos los obj hijos al padre mediante el metodo addchild
		root->addChild(mesh);
		root->addChild(mesh2);
	}
    
    void View::update ()
    {
		//llamamos al metodo update del obj padre
		root->update(projection,0.50f);
    }

    void View::paint ()
    {
		// Se borra el framebúffer y se dibujan los triángulos:
		rasterizer.clear();

		//llamo al metodo paint del obj padre
		root->paint(rasterizer);

		// Se copia el framebúffer oculto en el framebúffer de la ventana:
		rasterizer.get_color_buffer().gl_draw_pixels(0, 0);
	}

   

}
