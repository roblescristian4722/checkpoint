#ifndef GYM_H
#define GYM_H
#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <stdexcept>
#define ARCHIVO_DATOS "datos.dat"
#if _WIN64 || _WIN32
    #define CLEAR system("cls")
#else
    #define CLEAR system("clear")
#endif

struct Usuario
{
    char nombre[30];
    char apellido[30];
    unsigned int id;
    float peso;
    float altura;
    char ingreso[15];
};

struct Gym
{
    unsigned int ids;
    std::vector<Usuario> usrs;
    std::vector<unsigned int> idDis;

    Gym() { this->ids = 1; }
    ~Gym() {}
    void registrar();
    void guardar();
    bool recuperar();
    void imprimir();
    void borrar();
};

#endif // GYM_H
