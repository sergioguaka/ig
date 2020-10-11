#include "aux.h"
#include "malla.h"
#include "cubo.h"

Cubo::Cubo(float lado)
{

   // inicializar la tabla de vértices
   v.push_back(Tupla3f(   0,    0, lado));
   v.push_back(Tupla3f(lado,    0, lado));
   v.push_back(Tupla3f(   0, lado, lado));
   v.push_back(Tupla3f(lado, lado, lado));
   v.push_back(Tupla3f(   0,    0,    0));
   v.push_back(Tupla3f(lado,    0,    0));
   v.push_back(Tupla3f(   0, lado,    0));
   v.push_back(Tupla3f(lado, lado,    0));


   // inicializar la tabla de caras o triángulos:
   // (es importante en cada cara ordenar los vértices en sentido contrario
   //  de las agujas del reloj, cuando esa cara se observa desde el exterior del cubo)

   f.push_back(Tupla3i(0, 1, 2));
   f.push_back(Tupla3i(1, 3, 2));
   f.push_back(Tupla3i(2, 3, 7));
   f.push_back(Tupla3i(2, 7, 6));
   f.push_back(Tupla3i(1, 7, 3));
   f.push_back(Tupla3i(1, 5, 7));
   f.push_back(Tupla3i(6, 7, 4));
   f.push_back(Tupla3i(7, 5, 4));
   f.push_back(Tupla3i(0, 4, 1));
   f.push_back(Tupla3i(1, 4, 5));
   f.push_back(Tupla3i(2, 6, 4));
   f.push_back(Tupla3i(0, 2, 4));
   
}

