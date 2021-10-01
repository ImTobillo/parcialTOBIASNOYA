#include <stdio.h>
#include <stdlib.h>                                                             /// TOBIAS NOYA
#include "string.h"
#include "Fila.h"
#define personasArch "personas.bin"
#define boletosArch "boletos.bin"
#define boletoGanadorArch "boletoganador.bin"

/// estructuras

typedef struct
{
    char nombre[20];
    char DNI[9];
    int nro_carton;
} Persona;

typedef struct nodo
{
    Telemento dato;
    struct nodo* sig;
}nodo;

/// funciones

/// 1)

void inicLista (nodo** lista)
{
    *lista = NULL;
}

nodo* crearNodo (Telemento dato)
{
    nodo* aux = (nodo*)malloc(sizeof(nodo));
    aux->dato = dato;
    aux->sig = NULL;
    return aux;
}

void insertarAlFinalRec (nodo** lista, nodo* nuevo)
{
    if (*lista)
        insertarAlFinalRec(&(*lista)->sig, nuevo);
    else
        *lista = nuevo;
}

void despersistirYAgregarLista (nodo** lista)
{
    FILE* fpP = fopen (personasArch, "rb");
    FILE* fpB = fopen (boletosArch, "rb");

    Boleto boletoBuff;
    Persona personaBuff;
    BoletoPersona boletoPersonaBuff;

    if (fpP && fpB)
    {
        while (fread(&personaBuff, sizeof(Persona), 1, fpP) > 0)
        {
            strcpy(boletoPersonaBuff.nombre, personaBuff.nombre);
            strcpy(boletoPersonaBuff.DNI, personaBuff.DNI);

            while (fread(&boletoBuff, sizeof(Boleto), 1, fpB) > 0)
            {
                if (boletoBuff.nro_carton == personaBuff.nro_carton)
                    boletoPersonaBuff.boleto = boletoBuff;
            }

            insertarAlFinalRec(lista, crearNodo(boletoPersonaBuff));

            rewind(fpB);
        }

        fclose(fpP);
        fclose(fpB);
    }
    else
        printf("ERROR DE DATOS.\n");
}

void despersistirBoletoGanador (Boleto* boletoG)
{
    FILE* fp = fopen(boletoGanadorArch, "rb");

    if (fp)
    {
        fread(boletoG, sizeof(Boleto), 1, fp);
        fclose(fp);
    }
    else
        printf("ERROR DE DATOS.\n");
}

/// 2)

void borrarGanadorLista (nodo** lista, BoletoPersona dato)
{
    if (*lista)
    {
        nodo* aux = *lista;

        if ((*lista)->dato.boleto.nro_carton == dato.boleto.nro_carton)
        {
            *lista = (*lista)->sig;
            free(aux);
        }
        else
            borrarGanadorLista(&(*lista)->sig, dato);
    }
}

int retornarCantAciertos (BoletoPersona dato, Boleto boletoGanador)
{
    int cantAciertos = 0;

    for (int i = 0; i < 4; i++)
    {
        if (dato.boleto.num[i] == boletoGanador.num[i])
            cantAciertos++;
    }

    return cantAciertos;
}

void informarGanadorYEliminarDeLista (nodo** lista, Boleto boletoGanador)
{
    nodo* aux = *lista;

    while (retornarCantAciertos(aux->dato, boletoGanador) != 4) /// lo hago de esta forma porque tengo la certeza de que hay un ganador
        aux = aux->sig;

    printf("\nLA GANADORA ES.. %s\n", aux->dato.nombre);
    borrarGanadorLista(lista, aux->dato);
}

/// 3)

void agregar2doPremioY3Premio (nodo** lista, Fila* prem2, Fila* prem3, Boleto boletoGanador)
{
    nodo* aux = *lista;

    int cantAcrts;

    while (aux)
    {
        cantAcrts = retornarCantAciertos(aux->dato, boletoGanador);

        if (cantAcrts == 3)
        {
            poneFila(prem2, aux->dato);
            borrarGanadorLista(lista, aux->dato);
        }
        else if (cantAcrts == 2)
        {
            poneFila(prem3, aux->dato);
            borrarGanadorLista(lista, aux->dato);
        }

        aux = aux->sig;
    }
}

/// 4)

void mostrarFilaGanadoresRec2 (Fila* f, Fila* aux)
{
    if (!filaVacia(*f))
    {
        BoletoPersona bolPer = frente(*f);
        printf("%s\n", bolPer.nombre);
        poneFila(aux, sacaFila(f));
        mostrarFilaGanadoresRec2(f, aux);
    }
}

void mostrarFilaGanadoresRec (Fila* f)
{
    Fila aux;
    inicFila(&aux);

    mostrarFilaGanadoresRec2(f, &aux);

    while (!filaVacia(aux))
        poneFila(f, sacaFila(&aux));
}


/// main

int main()
{
    Boleto boletoGanador;
    despersistirBoletoGanador(&boletoGanador);

    nodo* lista;
    inicLista(&lista);

    despersistirYAgregarLista(&lista);

    informarGanadorYEliminarDeLista(&lista, boletoGanador);

    Fila premios2;
    Fila premios3;

    inicFila(&premios2);
    inicFila(&premios3);

    agregar2doPremioY3Premio(&lista, &premios2, &premios3, boletoGanador);

    printf("\nPREMIOS 2:\n");
    mostrarFilaGanadoresRec(&premios2);
    printf("\nPREMIOS 3:\n");
    mostrarFilaGanadoresRec(&premios3);

    return 0;
}
