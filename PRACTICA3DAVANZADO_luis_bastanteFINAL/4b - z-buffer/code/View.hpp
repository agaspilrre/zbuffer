/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Autor: Luis Bastante Fraile                                                *
*                                                                             *
*  Proyecto: Practica 3D avanzado version 1.0.                                *
*                                                                             *
*  Fecha: 27/01/2018                                                          *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//CLASE VIEW
//clase encargada de generar el viewport
//asigna el tamaño de viewport
//crea objetos mesh pasandoles su ruta y guarda los obj hijos en el padre

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <vector>
    #include <Point.hpp>
    #include "Rasterizer.hpp"
	#include <Projection.hpp>
	#include <list>
	#include "Mesh.hpp"

    namespace example
    {

        using std::vector;
        using toolkit::Point4i;
        using toolkit::Point4f;
		using toolkit::Projection3f;

		//rutas de los obj que queremos cargar
		static const string pathTree = "../../archivosObj/tree.obj";
		static const string pathHouse = "../../archivosObj/House.obj";
		static const string pathfloor = "../../archivosObj/floortile.obj";

        class View
        {

        private:
			
			//buffers de colores
			typedef Color_Buffer_Rgba8888 Color_Buffer;
			typedef Color_Buffer::Color   Color;

			//variable de tipo mesh que va a guardar el obj padre
			Mesh *root;

			Color_Buffer  Color_buffer;
			Rasterizer< Color_Buffer > rasterizer;

			//variables que van a guardar el tamaño del view
			size_t width;
			size_t height;

			Projection3f  projection;

        public:

			//metodo constructor que recibe como parametro la anchura y altura que debe tener el view
            View(size_t width, size_t height);

            void update ();
            void paint  ();

        };

    }

#endif
