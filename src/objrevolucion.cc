#include "aux.h"
#include "objrevolucion.h"
#include "ply_reader.h"
#include <fstream>
#include <limits>




// *****************************************************************************
//
// Clase ObjRevolucion (práctica 2)
//
// *****************************************************************************


// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un PLY)

ObjRevolucion::ObjRevolucion() {}

ObjRevolucion::ObjRevolucion(const std::string & archivo, int num_instancias, int eje, bool tapa_sup, bool tapa_inf) {

   unsigned num_vertices = 0, num_caras = 0;
   std::vector<Tupla3f> vertices;
   std::ifstream src;
   std::string na = archivo;

   // lee los datos del archivo
   ply::abrir_archivo(na, src);
   ply::leer_cabecera(src, num_vertices, num_caras, false);
   ply::leer_vertices(num_vertices, vertices, src);

   crearMalla(vertices, num_instancias, eje, tapa_sup, tapa_inf);
}

// *****************************************************************************
// objeto de revolución obtenido a partir de un perfil (en un vector de puntos)
ObjRevolucion::ObjRevolucion(std::vector<Tupla3f> archivo, int num_instancias, int eje, bool tapa_sup, bool tapa_inf) {
   crearMalla(archivo, num_instancias, eje, tapa_sup, tapa_inf);
}

void ObjRevolucion::crearMalla(std::vector<Tupla3f> perfil_original, int num_instancias, int eje, bool tapa_sup, bool tapa_inf)
{
   const float ALPHA = (2.0*M_PI) / (float)num_instancias;
   std::vector<Tupla3f> perfil_ascendente;
   Tupla3f aux, centroSup;

   // comprueba que los puntos se dan en orden ascendente (invierte si no)
   if (perfil_original.front()(eje) < perfil_original.back()(eje)) {
      perfil_ascendente = perfil_original;
   } else {
      for (auto p : perfil_original) {
         perfil_ascendente.insert(perfil_ascendente.begin(), p);
      }
   }

   // comprueba si tiene los centros de las tapas (añade si no)
   // tapa inferior
   aux = perfil_ascendente.front();
   aux(eje) = 0;
   if (aux.lengthSq() > std::numeric_limits<float>::epsilon()) {
      // la distancia no es 0 (hay que añadir el centro)
      aux = {0, 0, 0};
      aux(eje) = perfil_ascendente.front()(eje);
      perfil_ascendente.insert(perfil_ascendente.begin(), aux);
   }
   // tapa superior
   aux = perfil_ascendente.back();
   aux(eje) = 0;
   if (aux.lengthSq() > std::numeric_limits<float>::epsilon()) {
      // la distancia no es 0 (hay que añadir el centro)
      aux = {0, 0, 0};
      aux(eje) = perfil_ascendente.back()(eje);
      perfil_ascendente.push_back(aux);
   }

   ancho = num_instancias;
   alto = perfil_ascendente.size()-2;

   // calcula las distancias de los puntos al eje de rotación
   std::vector<float> modulos;
   for (auto p : perfil_ascendente) {
      p(eje) = 0;
      modulos.push_back(sqrt(p.lengthSq()));
   }

   // rellena el vector de vértices
   const unsigned int I = num_instancias;
   const unsigned int J = perfil_ascendente.size()-2; // sin las tapas
   const unsigned int TOTAL = I*J;

   // v.push_back(perfil_ascendente.front());
   for (unsigned int i = 0; i <= I; i++) {
      for (unsigned int j = 0; j < J; j++) {
         aux(eje) = (perfil_ascendente[j+1])(eje);
         aux((eje+1)%3) = modulos[j+1] * sin(ALPHA*i);
         aux((eje+2)%3) = modulos[j+1] * cos(ALPHA*i);

         v.push_back(aux);
      }
   }
   v.push_back(perfil_ascendente.front());
   v.push_back(perfil_ascendente.back());


   // rellena el vector de caras
   /*
   for (unsigned int i = 0; i < I; i++) {
      for (unsigned int j = 0; j < J-1; j++) {
         f.emplace_back(
            1 + (J*i     + j  )%TOTAL,
            1 + (J*i     + j+1)%TOTAL,
            1 + (J*(i+1) + j  )%TOTAL
         );
         f.emplace_back(
            1 + (J*i     + j+1)%TOTAL,
            1 + (J*(i+1) + j+1)%TOTAL,
            1 + (J*(i+1) + j  )%TOTAL
         );
      }
   }
   */
   for (unsigned int i = 0; i < I; i++) {
      for (unsigned int j = 0; j < J-1; j++) {
         f.emplace_back(
            (J*i     + j  )%TOTAL,
            (J*i     + j+1)%TOTAL,
            (J*(i+1) + j  )
         );
         f.emplace_back(
            (J*i     + j+1)%TOTAL,
            (J*(i+1) + j+1),
            (J*(i+1) + j  )
         );
      }
   }

   tamSinTapas = f.size();
   tamParSinTapas = ceil(f.size()/2);
   tamImpSinTapas = floor(f.size()/2);
   creaTapas(perfil_ascendente, num_instancias, eje);
}

void ObjRevolucion::creaTapas(
   std::vector<Tupla3f> perfil_original,
   int num_instancias,
   int eje,
   bool tapa_sup,
   bool tapa_inf
) {
   const unsigned int I = num_instancias;
   const unsigned int J = perfil_original.size()-2;
   const unsigned int TOTAL = I*J;
   Tupla3i aux;

   if (tapa_inf) {
      for (unsigned int i = 0; i < I; i++) {
         aux(eje)       = TOTAL+J;
         aux((eje+1)%3) = J*i;
         aux((eje+2)%3) = (J*(i+1))%TOTAL;

         f.push_back(aux);
      }
   }

   if (tapa_sup) {
      for (unsigned int i = 0; i < I; i++) {
         aux(eje)       = TOTAL+J+1;
         aux((eje+1)%3) = (J*(i+1) + J-1)%TOTAL;
         aux((eje+2)%3) = J*i      + J-1;
         
         f.push_back(aux);
      }
   }
}

// TBD
ObjRevolucion::ObjRevolucion(Tupla3f p, int num_instancias, int eje, bool tapa_sup)
{
   Tupla3f aux(p);
   float alpha = (2.0*M_PI) / (float)num_instancias;

   // Calcula la distancia del eje al punto
   aux(eje) = 0;
   float modulo = sqrt(aux.lengthSq());

   // añade el centro al array de vértices
   aux(0) = 0; aux(1) = 0; aux(2) = 0;
   aux(eje) = p(eje);
   v.push_back(aux);

   // añade el resto de puntos al array de vértices (eye Y)
   for (unsigned int i = 0; i < num_instancias; i++)
   {
      aux((eje+2)%3) = modulo * cos(alpha*(float)i);
      aux(eje) = p(eje);
      aux((eje+1)%3) = modulo * sin(alpha*(float)i);

      v.push_back(aux);
   }

   // crea el vector de caras
   if (tapa_sup) {
      for (unsigned int i = 0; i < num_instancias; i++)
         f.push_back(Tupla3i(i+1, 0, 1+(i+1)%num_instancias));
   } else {
      for (unsigned int i = 0; i < num_instancias; i++)
         f.push_back(Tupla3i(0, i+1, 1+(i+1)%num_instancias));
   }
}

// dibuja primero las tapas y luego el ObjRevolución
void ObjRevolucion::draw(unsigned int modo_vis, bool vbo, bool tapas)
{
   if (tapas) {
      // usa f.size()
      tam = -1;
      tam_imp = -1;
      tam_par = -1;
   } else {
      // usa los tamaños sin tapas
      tam = tamSinTapas;
      tam_imp = tamImpSinTapas;
      tam_par = tamParSinTapas;
   }
   // dibuja el cuerpo
   Malla3D::draw(modo_vis, vbo);
}


Cilindro::Cilindro (
   const int num_vert_perfil,
   const int num_instancias_perf,
   const float altura,
   const float radio
) {
   std::vector<Tupla3f> perfil;

   for (unsigned int i = 0; i < num_vert_perfil; i++) {
      perfil.emplace_back (
         radio,
         altura*((float)i/(num_vert_perfil-1)) - altura/2,
         0
      );
   }

   crearMalla(perfil, num_instancias_perf, 1, true, true);
   calculaTexturas();
}


Cono::Cono (
   const int num_vert_perfil,
   const int num_instancias_perf,
   const float altura,
   const float radio
) {
   std::vector<Tupla3f> perfil;

   for (unsigned int i = 0; i < num_vert_perfil; i++) {
      perfil.emplace_back (
         radio*((float)(num_vert_perfil-i-1)/(num_vert_perfil)),
         altura*((float)i/(num_vert_perfil-1)),
         0
      );
   }

   crearMalla(perfil, num_instancias_perf, 1, false, true);
}


Esfera::Esfera (
   const int num_vert_perfil,
   const int num_instancias_perf,
   const float radio
) {
   std::vector<Tupla3f> perfil;
   float alpha = (M_PI) / (float)(num_vert_perfil-1);

   for (unsigned int i = 0; i < num_vert_perfil; i++)
   {
      perfil.emplace_back (
         radio * sin(i*alpha),
         radio * cos(i*alpha),
         0
      );
   }

   crearMalla(perfil, num_instancias_perf, 1, false, false);
}

void ObjRevolucion::calculaTexturas() {
   const unsigned int I = ancho;
   const unsigned int J = alto; // sin las tapas
   const unsigned int TOTAL = I*J;

   printf("%d, %d\n", ancho, alto);

   for (unsigned int i = 0; i <= I; i++) {
      for (unsigned int j = 0; j < J; j++) {
         c_t.emplace_back((1-(float)(i)/(I)), 1-(float)(j)/(J-1));
      }
   }
}