/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Autor: Luis Bastante Fraile                                                *
*                                                                             *
*  Proyecto: Practica 3D avanzado version 1.0.                                *
*                                                                             *
*  Fecha: 27/01/2018                                                          *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

////CLASE MESH
////clase encargada de cargar archivos obj a traver del tini_obj,colorer los obj
////, darles posicionamiento escala y rotacion para el viewport y pintarlos en dicho viewport

#pragma once
#include <vector>
#include <Point.hpp>
#include "Rasterizer.hpp"
#include <Projection.hpp>
#include "Color_Buffer_Rgb565.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include<list>
#include<memory>

using namespace std;
using toolkit::Projection3f;
using toolkit::Transformation3f;

namespace example
{
	class Mesh {

		typedef Color_Buffer_Rgba8888 Color_Buffer;
		typedef Color_Buffer::Color   Color;

	private:

		//lista para la implementacion del grafo, guarda los obj hijos
		list<shared_ptr<Mesh>> children;

		typedef Point4f               Vertex;
		typedef vector< Vertex >      Vertex_Buffer;
		typedef vector< int    >      Index_Buffer;
		typedef vector< Color  >      Vertex_Colors;

		//listas para guardar los vertices cargados del obj, los indices, colores,transform...
		Vertex_Buffer     original_vertices;
		Index_Buffer      original_indices;
		Vertex_Colors     original_colors;
		Vertex_Buffer     transformed_vertices;
		vector< Point4i > display_vertices;

		//variables que otorgan la posicion y la escala del obj que se carga
		float positionX;
		float positionY;
		float positionZ;
		float meshScale;

		//variables para comprobar el objeto que tenemos que colorear
		string pathTree = "../../archivosObj/tree.obj";
		string pathHouse = "../../archivosObj/House.obj";
		string pathfloor = "../../archivosObj/floortile.obj";

	public:

		//constructor
		//recibe las posiciones en el eje X Y Z que va ha tener el obj en el view
		//recibe la escala que debe tener el obj cargado
		Mesh(const std::string & _objPath,float _positionX, float _positionY,float _positionZ, float _scaleObj);
		
		//metodo update de mesh que recibe como parametro el projection del view y un anguloX para la rotacion
		void update(Transformation3f & _parent_t, float _angleX);

		//metodo paint que recibe como parametro el rasterizer del metodo view
		void paint(Rasterizer< Color_Buffer > & rasterizer);
		
		//Metodos para implementar el grafo
		//añade obj hijos a la lista de hijos
		void addChild(shared_ptr<Mesh>&_child)
		{
			children.push_back(_child);
		}

	private:

		bool is_frontface(const Vertex * const projected_vertices, const int * const indices);

	};

}