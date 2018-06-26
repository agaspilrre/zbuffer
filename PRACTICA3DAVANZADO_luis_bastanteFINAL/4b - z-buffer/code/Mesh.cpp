/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
*                                                                             *
*  Autor: Luis Bastante Fraile                                                *
*                                                                             *
*  Proyecto: Practica 3D avanzado version 1.0.                                *
*                                                                             *
*  Fecha: 27/01/2018                                                          *
*                                                                             *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Mesh.hpp"
#include <cmath>
#include <cassert>
#include <Vector.hpp>
#include <Scaling.hpp>
#include <Rotation.hpp>
#include <Projection.hpp>
#include <Translation.hpp>

using namespace toolkit;

namespace example
{

	Mesh::Mesh(const std::string & _objPath,float _positionX,float _positionY, float _positionZ, float _scaleObj)
	{

		//guardamos los parametros recibidos en las variables de posicionamiento y escala
		positionX = _positionX;
		positionY = _positionY;
		positionZ = _positionZ;
		meshScale = _scaleObj;

		//variable que guarda los atributos del obj cargado
		tinyobj::attrib_t attrib;
		
		//variable que guarda las shapes
		std::vector<tinyobj::shape_t> shapes; //hay que coger la mesh que hay en la posicion 0
		//del mesh tienes que coger sus indices y dentro de indices hay unos que se llaman vertexindex por cada vertex index hay q meterlos en una lista
		std::vector<tinyobj::material_t> materials;
		std::string err;

		//cargamos el obj a partir de la ruta que recibimos como parametro
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, _objPath.c_str());

		//si no encuentra obj en la ruta da error
		if (!ret)
		{
			exit(1);
		}

		//volcamos los vertices del obj cargado en el array de vertices de nuestra clase
		const size_t vertex = attrib.vertices.size();

		for (size_t i = 0; i < vertex; i += 3)
		{
			//por cada tres vertices creo un point4 y se lo paso a original vertices
			original_vertices.push_back( Point4f ({ attrib.vertices[i], attrib.vertices[i + 1], attrib.vertices[i + 2], 1 }));
		}

		//volcamos los indices de la shape 0 ya que son obj de una unica mesh, en el array de indices
		for (auto const& index : shapes[0].mesh.indices)
		{
			original_indices.push_back(index.vertex_index);	
		}

		//redimensionamos los demas arrays al tamaño que tiene el original vertices.
		display_vertices.resize(original_vertices.size());
		transformed_vertices.resize(original_vertices.size());
		original_colors.resize(original_vertices.size());

		//coloreado de los objetos 
		//utilizamos las variables string para compararlas con la ruta obtenida como parametro para saber que objeto estamos coloreando
		//cada obj esta compuesto de dos colores distintivos
		//colocamos los colores comprobando el numero de vertice donde nos encontramos utilizando el original colors ya que este tiene mismo tamaño q vertices
		const size_t colorSize = original_colors.size();

		if (_objPath == pathTree)
		{
			for (size_t index = 0; index < colorSize; ++index)
			{
				if (index>18)
				{
					original_colors[index].set(66, 244, 66);
				}

				else
				{
					original_colors[index].set(239, 121, 2);
				}
			}
		}

		else if (_objPath == pathHouse)
		{
			for (size_t index = 0; index < colorSize; ++index)
			{
				if (index>18)
				{
					original_colors[index].set(100, 100, 109);
				}

				else
				{
					original_colors[index].set(239, 30, 2);
				}
			}
		}

		else
		{
			for (size_t index = 0; index < colorSize; ++index)
			{
				if (index>3)
				{
					original_colors[index].set(24, 142, 34);
				}

				else
				{
					original_colors[index].set(239, 166, 7);
				}
			}
		}
	
	}



	bool Mesh::is_frontface(const Vertex * const projected_vertices, const int * const indices)
	{
		// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
		// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:
		const Vertex & v0 = projected_vertices[indices[0]];
		const Vertex & v1 = projected_vertices[indices[1]];
		const Vertex & v2 = projected_vertices[indices[2]];

		return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
	}

	void Mesh:: update(Transformation3f & _parent_t,float _angleX)
	{
		//Projection3f & projection
		// Se actualizan los parámetros de transformatión (sólo se modifica el ángulo):
		static float angle = 0.f;
		
		angle += 0.025f;

		// Se crean las matrices de transformación:
		Scaling3f     scaling(meshScale);
		Rotation3f    rotation_x;
		Rotation3f    rotation_y;
		Translation3f translation(positionX, positionY, positionZ);

		//se establece como rotacion x el angulo recibido como parametro
		//para los hijos obj se tendra una rotacion 0 ya que no queremos tener la misma inclinacion que el padre
		rotation_x.set< Rotation3f::AROUND_THE_X_AXIS >(_angleX);

		//para la rotacion en Y se comprueba si son los hijos o el padre el que queremos rotar mediante el angulo recibido como parametro
		//como los angulos en X de los hijos siemre van a ser 0, lo distinguimos de esa forma del padre
		//los hijos tienen rotacionY 0
		//el padre tiene la rotacion de angle actualizandose en cada llamada
		if(_angleX==0)
			rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(0);
		else
			rotation_y.set< Rotation3f::AROUND_THE_Y_AXIS >(angle);

		// Creación de la matriz de transformación unificada:
		Transformation3f transformation = _parent_t * translation *rotation_x * rotation_y * scaling;
	
		// Se transforman todos los vértices usando la matriz de transformación resultante:
		for (size_t index = 0, number_of_vertices = original_vertices.size(); index < number_of_vertices; index++)
		{
			// Se multiplican todos los vértices originales con la matriz de transformación y
			// se guarda el resultado en otro vertex buffer:
			Vertex & vertex = transformed_vertices[index] = Matrix44f(transformation) * Matrix41f(original_vertices[index]);

			// La matriz de proyección en perspectiva hace que el último componente del vector
			// transformado no tenga valor 1.0, por lo que hay que normalizarlo dividiendo:
			float divisor = 1.f / vertex[3];

			vertex[0] *= divisor;
			vertex[1] *= divisor;
			vertex[2] *= divisor;
			vertex[3] = 1.f;
		}

		//vuelve a llamar al metodo update por cada obj hijo
		//le pasa como parametro el transformation del padre
		//y el angulo de rotacion en x, en este caso es 0.
		for (auto &child : children)
		{
			child->update(transformation,0.0f);
		}
	}

	void Mesh ::paint(Rasterizer< Color_Buffer > & rasterizer)
	{
		// Se convierten las coordenadas transformadas y proyectadas a coordenadas
		// de recorte (-1 a +1) en coordenadas de pantalla con el origen centrado.
		// La coordenada Z se escala a un valor suficientemente grande dentro del
		// rango de int (que es lo que espera fill_convex_polygon_z_buffer).
		float width  = (float)rasterizer.get_color_buffer().get_width  ();
		float height = (float)rasterizer.get_color_buffer().get_height ();

		Scaling3f        scaling = Scaling3f(width / 2.f, height / 2.f, 100000000.f);
		Translation3f    translation = Translation3f(width / 2.f, height / 2.f, 0.f);
		Transformation3f transformation = translation * scaling;

		for (size_t index = 0, number_of_vertices = transformed_vertices.size(); index < number_of_vertices; index++)
		{
			display_vertices[index] = Point4i(Matrix44f(transformation) * Matrix41f(transformed_vertices[index]));
		}

		for (int * indices = original_indices.data(), *end = indices + original_indices.size(); indices < end; indices += 3)
		{
			if (is_frontface(transformed_vertices.data(), indices))
			{
				// Se establece el color del polígono a partir del color de su primer vértice:
				rasterizer.set_color(original_colors[*indices]);

				// Se rellena el polígono:
				rasterizer.fill_convex_polygon_z_buffer(display_vertices.data(), indices, indices + 3);
			}
		}

		// por cada hijo obj se vuelve a llamar al metodo paint con el rasterizer del padre, GRAFO
		for (auto & child : children)
		{
			child->paint(rasterizer);
		}
	}


}


