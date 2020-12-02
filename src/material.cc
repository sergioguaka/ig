#include "material.h"

Material::Material (
    Tupla4f ambiente,
    Tupla4f difuso,
    Tupla4f especular,
    float brillo
) {
    this->ambiente = ambiente;
    this->difuso = difuso;
    this->especular = especular;
    this->brillo = brillo;
}

void Material::aplicar () {
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambiente);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difuso);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, especular);
}