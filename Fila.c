#include "Fila.h"

void inicFila(Fila* f)
{
 (*f).prim = NULL;
 (*f).ult = NULL;
}


nodoD* crearNodoD(Telemento dato)
{
    nodoD* nuevo = (nodoD*)malloc(sizeof(nodoD));
    nuevo->ant=NULL;
    nuevo->sig=NULL;
    nuevo->dato = dato;
    return nuevo;
}


void poneFila(Fila* f, Telemento dato)
{
    nodoD* nuevo = crearNodoD(dato);

   if (f->prim) // si fila tiene datos
   {
       f->ult->sig = nuevo;
       nuevo->ant = f->ult;
       f->ult = nuevo;
   }
   else
   {
       f->prim = nuevo;
       f->ult = nuevo;
   }
}


Telemento sacaFila(Fila* f)
{
    Telemento ret = f->prim->dato;
    nodoD* aux = f->prim;

   if (f->prim == f->ult)   /// si el primero es igual al ultimo, o sea solo hay un dato
   {
       inicFila(f);
   }
   else
   {
       f->prim = f->prim->sig;
       f->prim->ant = NULL;
   }

   free(aux);
   return ret;
}


//retorna 1 si esta vacia, sino 0
int filaVacia(Fila f)
{
    return (f.prim == NULL);

}

Telemento frente(Fila f)
{
    return f.prim->dato;
}







