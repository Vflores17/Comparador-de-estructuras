#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <cctype> 
#include <cstdlib>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h> 
#include <math.h>
#include <sstream>
#include <random>
#define MAX 4
#define MIN 2

typedef struct T_Votante { //Se define la estructura para los votantes
	char cedula[10];
	char codelec[7];
	char sexo;
	char fecha[9];
	char numjun[6];
	char nombre[31];
	char papellido[27];
	char sapellido[27];
}*PtrT_Votante;

typedef struct NodoAVL
{
	T_Votante* votante;
	NodoAVL * izq;
	NodoAVL * derecha;
	int altura;
}*Ptr_nodoAVL;



typedef struct NodoBB
{
	T_Votante* votante;
	NodoBB * HijoIzquierdo;
	NodoBB * HijoDerecho;
}X;

struct NodoBtree {
	T_Votante * val[MAX + 1];
	NodoBtree * link[MAX + 1];
	int count;
};

typedef struct nodoSimple {
	T_Votante * votante;
	nodoSimple* siguiente;

}*Ptr_nodoLista;

// ---------------------------------------------------------------  definicion y manejo del arbol binairo de busqueda ---------------------------------------------------------------------


bool InsertarBB ( NodoBB *& Raiz, T_Votante * nuevoVotante ) {
	if ( Raiz == NULL ) {
		Raiz = new NodoBB;
		Raiz->votante = nuevoVotante;
		Raiz->HijoIzquierdo = NULL;
		Raiz->HijoDerecho = NULL;
		return true;
	}
	else {
		if ( strcmp ( nuevoVotante->cedula, Raiz->votante->cedula ) == 0 ) { return false; } // Claves duplicadas no permitidas
		else if ( strcmp ( nuevoVotante->cedula, Raiz->votante->cedula ) < 0 ){ 
			return InsertarBB ( Raiz->HijoIzquierdo, nuevoVotante );
		}
		else{ 
			return InsertarBB ( Raiz->HijoDerecho, nuevoVotante );
		}
	}
}

void PodarHojas ( NodoBB *& Raiz ) {
	if ( Raiz != NULL ) {
		PodarHojas ( Raiz->HijoIzquierdo );
		PodarHojas ( Raiz->HijoDerecho );
		//printf ( "Borro : %s \n", Raiz->votante->cedula );  // Usar cédula como identificador en el mensaje
		delete Raiz;
		Raiz = NULL;
	}	
}

NodoBB * Buscar ( NodoBB * Raiz, const char * cedula ) {
	if ( Raiz == NULL ) {
		return NULL;
	}
	else {
		if ( strcmp ( cedula, Raiz->votante->cedula ) == 0 ) { return Raiz; }
		else if ( strcmp ( cedula, Raiz->votante->cedula ) < 0 ){ 
			return Buscar ( Raiz->HijoIzquierdo, cedula );
		}
		else { 
			return Buscar ( Raiz->HijoDerecho, cedula );
		}
	}
}

void EnOrdenIRD ( NodoBB * Raiz ) {
	if ( Raiz != NULL ) {
		EnOrdenIRD ( Raiz->HijoIzquierdo );
		printf ( "%s  ", Raiz->votante->cedula );
		EnOrdenIRD ( Raiz->HijoDerecho );
	}
}


// -----------------------------------------------------------------------------  definicion y manejo del arbol B ---------------------------------------------------------------------

//Crear nodo 
NodoBtree * crearnodo ( T_Votante * votante, NodoBtree * hijo , NodoBtree * root ) {
	NodoBtree * NuevoNodo = new NodoBtree;
	NuevoNodo->val[1] = votante;  // Almacenar el puntero a T_Votante en el índice 0
	NuevoNodo->count = 1;
	NuevoNodo->link[0] = root;  // Supongo que no necesitas 'root' aquí según el código original
	NuevoNodo->link[1] = hijo;
	return NuevoNodo;
}

//coloca el nodo en la posicion adecuada, de acuerdo a su valor
void Colocarnodo ( T_Votante * votante, int pos, NodoBtree *node, NodoBtree *hijo ) {
	int j = node->count;
	while ( j > pos ) {
		node->val[j + 1] = node->val[j];
		node->link[j + 1] = node->link[j];
		j--;
	}
	node->val[j + 1] = votante;
	node->link[j + 1] = hijo;
	node->count++;
}

void PartirNodo ( T_Votante * votante, T_Votante ** pval, int pos, NodoBtree * node, NodoBtree * hijo, NodoBtree ** NuevoNodo ) {
	int median, j;

	if ( pos > MIN ) {
		median = MIN + 1;
	}
	else {
		median = MIN;
	}

	*NuevoNodo = new NodoBtree;
	j = median + 1;
	while ( j <= MAX) {
		( *NuevoNodo )->val[j - median] = node->val[j];
		( *NuevoNodo )->link[j - median ] = node->link[j];
		j++;
	}

	node->count = median;
	( *NuevoNodo )->count = MAX - median;

	if ( pos <= MIN ) {
		Colocarnodo ( votante, pos, node, hijo );
	}
	else {
		Colocarnodo ( votante, pos - median, *NuevoNodo, hijo );
	}
	*pval = node->val[node->count];
	( *NuevoNodo )->link[0] = node->link[node->count];
	node->count--;
}

//colocar valor
int SetValorNodo ( T_Votante * votante, T_Votante ** pval, NodoBtree * node, NodoBtree ** hijo ) {
	int pos;
	if ( !node ) {
		*pval = votante;
		*hijo = NULL;
		return 1;
	}

	if ( strcmp ( votante->cedula, node->val[1]->cedula ) < 0 ) {
		pos = 0;
	}
	else {
		for ( pos = node->count; strcmp ( votante->cedula, node->val[pos]->cedula ) < 0 && pos > 1; pos-- );
		if ( strcmp ( votante->cedula, node->val[pos]->cedula ) == 0 ) {
			std::cout << "No esta permitido nodos duplicados\n";
			return 0;
		}
	}
	if ( SetValorNodo ( votante, pval, node->link[pos], hijo ) ) {
		if ( node->count < MAX ) {
			Colocarnodo ( *pval, pos, node, *hijo );
		}
		else {
			PartirNodo ( *pval, pval, pos, node, *hijo, hijo );
			return 1;
		}
	}
	return 0;
}

//inserta valores en el btree
void insertarBTREE ( T_Votante * votante, NodoBtree ** root ) {

	T_Votante * valorRetornado;  // Esto almacenará el valor "promovido" si es necesario
	NodoBtree * nuevoHijo;
	int flag; // Esto almacenará el nuevo nodo hijo si la raíz se divide

	// SetValorNodo ahora debe manejar correctamente los valores de T_Votante, por lo que devuelve si hubo una división y necesita promoción
	flag = SetValorNodo ( votante, &valorRetornado, *root, &nuevoHijo );

	if ( flag ) {  // Si hay promoción, significa que debemos crear un nuevo nodo raíz
		*root = crearnodo ( valorRetornado, nuevoHijo, *root );
	}
}

//copia el sucesor del nodo que será borrado
void copySuccessor ( NodoBtree * minodo, int pos ) {
	NodoBtree * dummy = minodo->link[pos];

	for ( ; dummy->link[0] != NULL;)
		dummy = dummy->link[0];
	minodo->val[pos] = dummy->val[1];
}

//remueve un valor de un nodo y reacomoda el arbol
void removeVal ( NodoBtree * minodo, int pos ) {
	int i = pos + 1;
	while ( i <= minodo->count ) {
		minodo->val[i - 1] = minodo->val[i];
		minodo->link[i - 1] = minodo->link[i];
		i++;
	}
	minodo->count--;
}

//shift a la derecha
void doRightShift ( NodoBtree * minodo, int pos ) {
	NodoBtree * x = minodo->link[pos];
	int j = x->count;

	while ( j > 0 ) {
		x->val[j + 1] = x->val[j];
		x->link[j + 1] = x->link[j];
	}
	x->val[1] = minodo->val[pos];
	x->link[1] = x->link[0];
	x->count++;

	x = minodo->link[pos - 1];
	minodo->val[pos] = x->val[x->count];
	minodo->link[pos] = x->link[x->count];
	x->count--;
	return;
}

//shift a la izquierda
void doLeftShift ( NodoBtree * minodo, int pos ) {
	int j = 1;
	NodoBtree * x = minodo->link[pos - 1];

	x->count++;
	x->val[x->count] = minodo->val[pos];
	x->link[x->count] = minodo->link[pos]->link[0];

	x = minodo->link[pos];
	minodo->val[pos] = x->val[1];
	x->link[0] = x->link[1];
	x->count--;

	while ( j <= x->count ) {
		x->val[j] = x->val[j + 1];
		x->link[j] = x->link[j + 1];
		j++;
	}
	return;
}

//Fusionar nodos
void UnirNodos ( NodoBtree * minodo, int pos ) {
	int j = 1;
	NodoBtree * x1 = minodo->link[pos], * x2 = minodo->link[pos - 1];

	x2->count++;
	x2->val[x2->count] = minodo->val[pos];
	x2->link[x2->count] = minodo->link[0];

	while ( j <= x1->count ) {
		x2->count++;
		x2->val[x2->count] = x1->val[j];
		x2->link[x2->count] = x1->link[j];
		j++;
	}

	j = pos;
	while ( j < minodo->count ) {
		minodo->val[j] = minodo->val[j + 1];
		minodo->link[j] = minodo->link[j + 1];
		j++;
	}
	minodo->count--;
	free ( x1 );
}

//Ajusta el nodo dado
void AjustarNodo ( NodoBtree * minodo, int pos ) {
	if ( !pos ) {
		if ( minodo->link[1]->count > MIN ) {
			doLeftShift ( minodo, 1 );
		}
		else {
			UnirNodos ( minodo, 1 );
		}
	}
	else {
		if ( minodo->count != pos ) {
			if ( minodo->link[pos - 1]->count > MIN ) {
				doRightShift ( minodo, pos );
			}
			else {
				if ( minodo->link[pos + 1]->count > MIN ) {
					doLeftShift ( minodo, pos + 1 );
				}
				else {
					UnirNodos ( minodo, pos );
				}
			}
		}
		else {
			if ( minodo->link[pos - 1]->count > MIN )
				doRightShift ( minodo, pos );
			else
				UnirNodos ( minodo, pos );
		}
	}
}

//borrar un valor del nodo
// Adaptando la función para buscar y borrar por cédula
int BorrarDeNodo ( const char * cedula, NodoBtree * minodo ) {
	int pos, flag = 0;
	if ( minodo ) {
		//
		if ( strcmp ( cedula, minodo->val[1]->cedula ) < 0 ) {
			pos = 0;
			flag = 0;
		}
		else {
			for ( pos = minodo->count;
				( strcmp ( cedula, minodo->val[pos]->cedula ) < 0 && pos > 1 ); pos-- );
			//
			if ( strcmp ( cedula, minodo->val[pos]->cedula ) == 0 ) {
				flag = 1;
			}
			else {
				flag = 0;
			}
		}
		if ( flag ) {
			if ( minodo->link[pos - 1] ) {
				copySuccessor ( minodo, pos );
				flag = BorrarDeNodo ( minodo->val[pos]->cedula, minodo->link[pos] );
				if ( flag == 0 ) {
					std::cout << "No existe en el B-Tree\n";
				}
			}
			else {
				removeVal ( minodo, pos );
			}
		}
		else {
			flag = BorrarDeNodo ( cedula, minodo->link[pos] );
		}
		if ( minodo->link[pos] ) {
			if ( minodo->link[pos]->count < MIN )
				AjustarNodo ( minodo, pos );
		}
	}
	return flag;
}

/* delete val from B-tree */
void borrado ( const char * cedula, NodoBtree * pminodo, NodoBtree * root ) {
	NodoBtree * tmp;
	if ( !BorrarDeNodo ( cedula, pminodo ) ) {
		std::cout << "No existe ese valor en el B-Tree\n";
		system ( "pause" );
		return;
	}
	else {
		if ( pminodo->count == 0 ) {
			tmp = pminodo;
			pminodo = pminodo->link[0];
			free ( tmp );
		}
	}
	root = pminodo;
	std::cout << "Borrado" << std::endl;
	system ( "pause" );
	return;
}

/* search val in B-Tree */
void busqueda ( const char * cedula, int * pos, NodoBtree * minodo ) {
	if ( !minodo ) {
		return;
	}
	//
	if ( strcmp ( cedula, minodo->val[1]->cedula ) < 0 ) {
		*pos = 0;
	}
	else {
		for ( *pos = minodo->count;
			//
			( strcmp ( cedula, minodo->val[*pos]->cedula ) < 0 && *pos > 1 ); ( *pos )-- );
		//
		if ( strcmp ( cedula, minodo->val[*pos]->cedula ) == 0 ) {
			std::cout << "Se ha encontrado el Nodo\n";
			system ( "pause" );
			return;
		}
	}
	busqueda ( cedula, pos, minodo->link[*pos] );
	std::cout << "No se ha encontrado el Nodo\n";
	system ( "pause" );
	return;
}

// B-Tree desplegar
void desplegar ( NodoBtree * minodo ) {
	int i;
	if ( minodo ) {
		for ( i = 0; i < minodo->count; i++ ) {
			desplegar ( minodo->link[i] );
			std::cout << minodo->val[i + 1] << ' ';
		}
		desplegar ( minodo->link[i] );
	}
}

void liberarArbolBTREE ( NodoBtree * nodo ) {
	if ( nodo == nullptr ) {
		return;  // Si el nodo es nullptr, no hay nada que liberar.
	}

	// Recorrer todos los subárboles recursivamente y liberarlos
	for ( int i = 0; i <= nodo->count; i++ ) {  // Incluir todos los enlaces posibles
		liberarArbolBTREE ( nodo->link[i] );
	}

	// Liberar el nodo actual sin tocar los datos T_Votante
	delete nodo;
}

// -----------------------------------------------------------------------------  definicion y manejo del arbol AVL ---------------------------------------------------------------------

//obtiene la altura del arbol
int altura ( Ptr_nodoAVL & N )
{
	if ( N == NULL )
		return 0;
	return N->altura;
}

Ptr_nodoAVL nuevoNodo ( T_Votante * votante ) {
	Ptr_nodoAVL Nodo = new NodoAVL;
	Nodo->votante = votante;
	Nodo->izq = NULL;
	Nodo->derecha = NULL;
	Nodo->altura = 1;  // nuevonodo es agregado como hoja
	return Nodo;
}

Ptr_nodoAVL RotarDerecha ( Ptr_nodoAVL & y )
{
	Ptr_nodoAVL x = y->izq;
	Ptr_nodoAVL T2 = x->derecha;

	// hace rotacion
	x->derecha = y;
	y->izq = T2;

	// actualiza alturas
	y->altura = max ( altura ( y->izq ), altura ( y->derecha ) ) + 1;
	x->altura = max ( altura ( x->izq ), altura ( x->derecha ) ) + 1;

	// retorna nueva raiz
	return x;
}

Ptr_nodoAVL RotarIzquierda ( Ptr_nodoAVL & x )
{
	Ptr_nodoAVL y = x->derecha;
	Ptr_nodoAVL T2 = y->izq;

	// hace rotacion
	y->izq = x;
	x->derecha = T2;

	//  actualiza alturas
	x->altura = max ( altura ( x->izq ), altura ( x->derecha ) ) + 1;
	y->altura = max ( altura ( y->izq ), altura ( y->derecha ) ) + 1;

	// retorna nueva raiz
	return y;
}

// obtiene balance
int obtenerbalance ( Ptr_nodoAVL & N )
{
	if ( N == NULL ) { 
		return 0;
	}
	return altura ( N->izq ) - altura ( N->derecha );
}

Ptr_nodoAVL insertarAVL ( Ptr_nodoAVL & Nodo, T_Votante * votante ) {
	if ( Nodo == NULL ){
		Nodo = nuevoNodo ( votante );
		return Nodo;
	}

	// Suponiendo que comparamos cédulas como cadenas
	if ( strcmp ( votante->cedula, Nodo->votante->cedula ) < 0 ){
		Nodo->izq = insertarAVL ( Nodo->izq, votante );
	}
	else if ( strcmp ( votante->cedula, Nodo->votante->cedula ) > 0 ){ 
		Nodo->derecha = insertarAVL ( Nodo->derecha, votante );
	}
	else {  // No se permiten llaves iguales, pero podrías manejar esto de otra manera
		return Nodo;
	}

	// Actualiza la altura del nodo padre
	Nodo->altura = 1 + max ( altura ( Nodo->izq ), altura ( Nodo->derecha ) );

	// Verifica si el árbol se ha desbalanceado
	int balance = obtenerbalance ( Nodo );

	// Realiza las rotaciones necesarias
	if ( balance > 1 && strcmp ( votante->cedula, Nodo->izq->votante->cedula ) < 0 ){ 
		return RotarDerecha ( Nodo );
	}
	if ( balance < -1 && strcmp ( votante->cedula, Nodo->derecha->votante->cedula ) > 0 ) { 
		return RotarIzquierda ( Nodo );
	}
	if ( balance > 1 && strcmp ( votante->cedula, Nodo->izq->votante->cedula ) > 0 ) {
		Nodo->izq = RotarIzquierda ( Nodo->izq );
		return RotarDerecha ( Nodo );
	}
	if ( balance < -1 && strcmp ( votante->cedula, Nodo->derecha->votante->cedula ) < 0 ) {
		Nodo->derecha = RotarDerecha ( Nodo->derecha );
		return RotarIzquierda ( Nodo );
	}

	return Nodo;
}

//recorido
void preOrder ( Ptr_nodoAVL & root )
{
	if ( root != NULL )
	{
		std::cout << "nombre: " << root->votante->nombre << std::endl;
		preOrder ( root->izq );
		preOrder ( root->derecha );
	}

}

void liberarArbolAVL ( Ptr_nodoAVL & nodo ) {
	if ( nodo != NULL ) {
		liberarArbolAVL ( nodo->izq );
		liberarArbolAVL ( nodo->derecha );
		delete nodo;
		nodo = NULL;
	}
}

// ------------------------------------------------------   estructura de T_Votante -----------------------------------------------------------------------------------------------------

//Funcion para agregar un votante, recibe como parametro la lista de votantes y la LINEA DE TEXTO del padron electoral
//extrae cada uno de los atributos de la linea y los almacena en un nodo de la lista de votantes
void agregarvotante ( Ptr_nodoLista & ListaV, char agregado[118] ) {

	PtrT_Votante nuevoVotante = new  T_Votante ;
	Ptr_nodoLista nuevoNodo = new nodoSimple ;

	nuevoNodo->votante = nuevoVotante;
	nuevoNodo->siguiente = ListaV;
	ListaV = nuevoNodo;

	for ( int ce = 0; ce < 9; ce++ ) {
		nuevoVotante->cedula[ce] = agregado[ce];
	}
	nuevoVotante->cedula[9] = '\0';

	for ( int co = 10; co < 16; co++ ) {
		nuevoVotante->codelec[co - 10] = agregado[co];
	}
	nuevoVotante->codelec[6] = '\0';

	nuevoVotante->sexo = agregado[17];


	for ( int fe = 19; fe < 27; fe++ ) {
		nuevoVotante->fecha[fe - 19] = agregado[fe];
	}
	nuevoVotante->fecha[8] = '\0';

	for ( int nu = 28; nu < 33; nu++ ) {
		nuevoVotante->numjun[nu - 28] = agregado[nu];
	}
	nuevoVotante->numjun[5] = '\0';

	for ( int nom = 34; nom < 64; nom++ ) {
		nuevoVotante->nombre[nom - 34] = agregado[nom];
	}
	nuevoVotante->nombre[30] = '\0';

	for ( int pa = 65; pa < 91; pa++ ) {
		nuevoVotante->papellido[pa - 65] = agregado[pa];
	}
	nuevoVotante->papellido[26] = '\0';

	for ( int sa = 92; sa < 118; sa++ ) {
		nuevoVotante->sapellido[sa - 92] = agregado[sa];
	}
	nuevoVotante->sapellido[26] = '\0';

}

//Funcion para cargar los votantes, se encarga de tomar cada una de las lineas
//directo del archivo de texto y enviarlo a la funci�n encargada de extraer cad uno
// de los datos e incorporarlos en un nodo de la lista.
void CargarVotantes ( Ptr_nodoLista &ListaV ) {
	int cont = 1;
	time_t inicio, fin;
	inicio = time ( NULL );
	FILE * archivo;
	fopen_s ( &archivo, "PADRON_COMPLETO.txt", "r" );
	if ( NULL == archivo ) {
		printf ( "No se pudo abrir el archivo" );
	}
	else {
		char agregado[119];
		while ( fgets ( agregado, 119, archivo ) != NULL ) {
			if ( cont % 2 != 0 ) {
				agregarvotante ( ListaV, agregado );
			}
			cont++;
		}
		fclose ( archivo );
	}

	fin = time ( NULL );
	std::cout << "----------------------------Padron cargado--------------------------------" << std::endl;
	printf ( "\nEl cargado del padron ha tardado : %f segundos.\n", difftime ( fin, inicio ) );
	std::cout << "----------------------------Padron cargado--------------------------------" << std::endl;
}

//Funcion para liberar los votantes en estructuras de memoria dinamica de la lista enlazada hasta dejar la lista en NULL
void LiberarVotantes ( Ptr_nodoLista & ListaV ) {
	Ptr_nodoLista aux = ListaV;
	//time_t inicio, fin;

	//inicio = time ( NULL );

	while ( aux != NULL ) {
		ListaV = ListaV->siguiente;
		delete ( aux->votante );
		delete( aux );
		aux = ListaV;
	}

	//fin = time ( NULL );

	//std::cout << "----------------------------Padron liberado--------------------------------" << std::endl;
	//printf ( "\nLa liberacion del padron ha tardado : %f segundos.\n", difftime ( fin, inicio ) );
	//std::cout << "----------------------------Padron liberado--------------------------------" << std::endl;
}

Ptr_nodoLista BuscarVotante ( Ptr_nodoLista & Lista, char cual[9] )
{
	bool encontro = false;
	Ptr_nodoLista Aux;
	Aux = Lista;

	while ( ( !encontro == true ) && ( Aux != NULL ) )
	{
		if ( strncmp ( Aux->votante->cedula, cual, 9 ) == 0 )
			encontro = true;
		else
			Aux = Aux->siguiente;
	}
	return Aux;
}

// ------------------------------------------------------   estructura de T_Votante y manejo de lista enlazada simple -------------------------------------------------------------------------

void mainEstructuras ( ) {
	Ptr_nodoLista listaVotante =NULL;
	//NodoBB * rootBB = NULL;  // Raíz del árbol binario de búsqueda
	//NodoAVL * rootAVL = NULL;  // Raíz del árbol AVL
	NodoBtree * rootBTREE = NULL;  // Raíz del árbol B

	std::cout << "Por favor espere mientras se carga el Padron" << std::endl;
	CargarVotantes ( listaVotante );
	Ptr_nodoLista aux = listaVotante;
	system ( "PAUSE" );
	time_t inicio, fin;
	inicio = time ( NULL );

	while ( aux != NULL ) {
		insertarBTREE (aux->votante, &rootBTREE );
		aux = aux->siguiente;
	}


	fin = time ( NULL );
	std::printf ( "Se tardo en crear el arbol: %f segundos\n", difftime ( fin, inicio ) );

	//std::cout << "imptimiendo btree" << std::endl;
	//desplegar ( rootBTREE );

	time_t iniciox, finx;
	iniciox = time ( NULL );

	
	std::cout << "eliminando el btree" << std::endl;
	liberarArbolBTREE ( rootBTREE );

	finx = time ( NULL );
	std::printf ( "Se elimino el arbol b, se duro: %f segundos", difftime ( finx, iniciox ) );

	LiberarVotantes ( listaVotante );
	std::printf ( "lista borrada" );
	

	std::cout << "Finalizado" << std::endl;



}

/*
				Logica para agregar y eliminar los elementos al arbol B a partir de la informacion de la lista enlazada ----------------------------------------------------
				time_t inicio, fin;
					inicio = time ( NULL );

					while ( aux != NULL ) {
						insertarBTREE (aux->votante, &rootBTREE );
						aux = aux->siguiente;
					}


					fin = time ( NULL );
					std::printf ( "Se tardo en crear el arbol: %f segundos\n", difftime ( fin, inicio ) );

					//std::cout << "imptimiendo btree" << std::endl;
					//desplegar ( rootBTREE );

					time_t iniciox, finx;
					iniciox = time ( NULL );


					std::cout << "eliminando el btree" << std::endl;
					liberarArbolBTREE ( rootBTREE );

					finx = time ( NULL );
					std::printf ( "Se elimino el arbol b, se duro: %f segundos", difftime ( finx, iniciox ) );


*/

/*

					Logica para agregar y enumerar los nodos del arbol avl de la informacion de la lista enlazada--------------------------------------------

					time_t inicio, fin;
					inicio = time ( NULL );
					while ( aux != NULL ) {
							insertarAVL ( rootAVL, aux->votante );
							aux = aux->siguiente;
						}


						fin = time ( NULL );
						std::printf ( "Se tardo en crear el arbol: %f segundos\n", difftime ( fin, inicio ) );
						system ( "PAUSE" );
						std::printf("Se muestra el contenido del arbol avl\n");
						preOrder ( rootAVL );


						time_t iniciox, finx;
						iniciox = time ( NULL );

						liberarArbolAVL ( rootAVL );
						std::printf ( "arbol podado" );

						finx = time ( NULL );

*/


/*
				Logica para poder insertar y eliminar el arbol binario de busqueda la informacion de la lista enlazada------------------------------

				while ( aux != NULL ) {
					InsertarBB ( rootBB, aux->votante );
					aux = aux->siguiente;
				}


				std::printf ( "se procede a eliminar el arbol\n" );
				time_t iniciox, finx;

				iniciox = time ( NULL );

				PodarHojas ( root );
				std::printf ( "arbol podado" );

				finx = time ( NULL );
				std::printf ( "Se podaron las hojas, se duro: %f segundos", difftime ( finx, iniciox ) );
				*/

				/*					Logica para poder hacer una busqueda en la lista enlazada --------------------------------
										

										char buscado[] = "118450474";
										Ptr_nodoLista Encontrado = NULL;
										Encontrado = BuscarVotante ( listaVotante, buscado );
										if ( Encontrado != NULL ) {
											std::cout << std::endl << "Encontre' al Votante!! " << std::endl << std::endl;

											std::cout << "Ce'dula: " << Encontrado->votante->cedula << std::endl;
											std::cout << "Nombre: " << Encontrado->votante->nombre << std::endl;
											std::cout << "Primer Apellido: " << Encontrado->votante->papellido << std::endl;
											std::cout << "Segundo Apellido: " << Encontrado->votante->sapellido << std::endl;
											std::cout << "Sexo: " << Encontrado->votante->sexo << std::endl;
											std::cout << "Fecha: " << Encontrado->votante->fecha << std::endl;
											std::cout << "Co'digo Electoral:  " << Encontrado->votante->codelec << std::endl;
											std::cout << "Nu'mero Junta: " << Encontrado->votante->numjun << std::endl;
										}
										else
											std::cout << " No se encontro al votante !!! " << std::endl;
											*/