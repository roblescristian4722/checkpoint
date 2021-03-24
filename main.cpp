#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include "gym.h"

int main()
{
    Gym gym;
    unsigned char aux = ' ';
    unsigned int tmp;
    bool run = true;
    
    CLEAR;
    if (gym.recuperar())
        std::cout << "Datos recuperados" << std::endl;
    while (run) {
        std::cout << "1) añadir"   << std::endl
                  << "2) borra"    << std::endl
                  << "3) imprimir" << std::endl
                  << "4) salir"    << std::endl << std::endl
                  << ">> ";
        std::cin >> aux;
        switch (aux) {
            case '1':
                gym.registrar();
            break;
            case '2':
                gym.borrar();
            break;
            case '3':
                gym.imprimir();
            break;
            case '4':
                run = false;
            break;
            default: break;
        }
        if (run) {
            std::cout << "Presione una tecla para continuar..." << std::endl;
            std::cin.get();
        }
        CLEAR;
    }

    return 0;
}
