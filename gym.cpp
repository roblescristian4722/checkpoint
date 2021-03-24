#include "gym.h"
#include <algorithm>
#include <cstring>
#include <stdexcept>

void Gym::imprimir()
{
    CLEAR;
    std::sort(this->usrs.begin(), this->usrs.end(),
              [] (const Usuario &a, const Usuario &b){return (a.id < b.id);});
    for (const Usuario &x: this->usrs)
        std::cout << "-------------------- Usuario: " << x.id
                  << " --------------------" << std::endl
                  << "Nombre: " << x.nombre << " " << x.apellido << std::endl
                  << "Peso: " << x.peso << std::endl
                  << "Altura: " << x.altura << std::endl
                  << "Fecha de ingreso: " << x.ingreso << std::endl
                  << std::endl;
    std::cin.ignore();
}

void Gym::borrar()
{
    std::vector<Usuario>::iterator it = this->usrs.begin();
    std::fstream archivo(ARCHIVO_DATOS,
            std::ios::in | std::ios::out | std::ios::binary);
    unsigned int tmp;
    Usuario vacio;
    vacio.id = -1;
    try {
        if (!this->usrs.size())
            throw std::range_error("No hay usuarios registrados");
        if (!archivo.is_open())
            throw std::invalid_argument("No se pudo abrir el achivo");
    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        std::cin.get();
        return;
    }
    std::cout << std::endl << "ID del usuario a eliminar: ";
    std::cin >> tmp;
    for (;it != this->usrs.end(); it++)
        if (it->id == tmp)
            break;
    if (it != this->usrs.end()) {
        archivo.seekg((it->id - 1) * sizeof(Usuario));
        archivo.write((char*)&vacio, sizeof(Usuario));
        this->idDis.push_back(it->id);
        this->usrs.erase(it);
    }
    else {
        std::cout << "No se encontró el usuario especificado..." << std::endl;
        std::cin.ignore();
    }
}

void Gym::registrar()
{
    Usuario usrTmp;
    std::string aux;
    std::regex nomApe("[\\w|ñ]{3,}");
    std::regex pes("[3-9]{1}\\d{1,2}(\\.\\d{0,2})?");
    std::regex alt("[5-9]{1}\\d{1,2}(\\.\\d{0,2})?");
    std::regex ing("([1-9]|([1-3]\\w))\\-([1-9]|(1[0-2]))\\-(20\\w{2})");

    std::cin.ignore();
    do {
        std::cout << "Intrese su nombre: ";
        getline(std::cin, aux);
    } while (!std::regex_match(aux, nomApe));
    strcpy(usrTmp.nombre, aux.c_str());

    do {
        std::cout << "Intrese su apellido: ";
        getline(std::cin, aux);
    } while (!std::regex_match(aux, nomApe));
    strcpy(usrTmp.apellido, aux.c_str());
    
    do {
        std::cout << "Intrese su peso actual (30-300 kg): ";
        getline(std::cin, aux);
    } while (!std::regex_match(aux, pes) || (std::stof(aux) > 300.0));
    usrTmp.peso = std::stof(aux);

    do {
        std::cout << "Intrese su altura (50-230cm): ";
        getline(std::cin, aux);
    } while (!std::regex_match(aux, alt) || (std::stof(aux) > 230.0));
    usrTmp.altura = std::stof(aux);

    do {
        std::cout << "Intrese su fecha de ingreso: ";
        getline(std::cin, aux);
    } while (!std::regex_match(aux, ing));
    strcpy(usrTmp.ingreso, aux.c_str());
    if (!this->idDis.size())
        usrTmp.id = this->ids++;
    else {
        usrTmp.id = this->idDis.front();
        this->idDis.erase(this->idDis.begin());
    }
    this->usrs.push_back(usrTmp);
    guardar();
}

void Gym::guardar()
{
    std::fstream archivo(ARCHIVO_DATOS, std::ios::out | std::ios::in
                                        | std::ios::binary);
    std::fstream errArch;
    std::string errorMsj = ARCHIVO_DATOS;
    
    try {
        if (!archivo.is_open()) {
            errorMsj += " no se ha podido abrir";
            throw std::out_of_range(errorMsj);
        }
    }
    catch(const std::out_of_range &e) {
        std::cout << e.what() << std::endl
                  << "Creando nuevo archivo..." << std::endl;
        errArch.open(ARCHIVO_DATOS, std::ios::out);
        errArch.write((char*)&this->usrs.back(), sizeof(Usuario));
        errArch.close();
        return;
    }
    archivo.seekg((this->usrs.back().id - 1) * sizeof(Usuario));
    archivo.write((char*)&this->usrs.back(), sizeof(Usuario));
    archivo.close();
}

bool Gym::recuperar()
{
    std::fstream archivo(ARCHIVO_DATOS, std::ios::in | std::ios::binary);
    std::fstream errArch;
    std::string errorMsj = ARCHIVO_DATOS;
    bool recup = false;
    Usuario usrTmp;
    
    try {
        if (!archivo.is_open()) {
            errorMsj += " no se ha podido abrir";
            throw std::out_of_range(errorMsj);
        }
    }
    catch(const std::out_of_range &e) {
        std::cout << e.what() << std::endl
                  << "Creando nuevo archivo..." << std::endl;
        errArch.open(ARCHIVO_DATOS, std::ios::out);
        errArch.close();
        return false;
    }
    while (!archivo.eof()) {
        archivo.read((char*)&usrTmp, sizeof(Usuario));
        if (archivo.eof())
            break;
        if (usrTmp.id != -1) {
            this->usrs.push_back(usrTmp);
            recup = true;
        }
        else
            this->idDis.push_back(archivo.tellg() / sizeof(Usuario));
    }
    this->ids = this->usrs.back().id + 1;
    return recup;
}
