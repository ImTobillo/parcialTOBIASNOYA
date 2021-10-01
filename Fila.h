#ifndef FILA_H_INCLUDED
#define FILA_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int nro_carton;
    char num[5];
} Boleto;

typedef struct
{
    char nombre[20];
    char DNI[9];
    Boleto boleto;
} BoletoPersona;


typedef BoletoPersona Telemento;

typedef struct nodoD
{
    Telemento dato;
    struct nodoD* ant;
    struct nodoD* sig;
} nodoD;


typedef struct
{
    nodoD* prim;
    nodoD* ult;
    ///COMPLETAR
} Fila;

///funciones
void inicFila(Fila*);
nodoD* crearNodoD(Telemento);
void poneFila(Fila*,Telemento);
Telemento sacaFila(Fila*);
int filaVacia(Fila);
Telemento frente(Fila);



#endif // FILA_H_INCLUDED
