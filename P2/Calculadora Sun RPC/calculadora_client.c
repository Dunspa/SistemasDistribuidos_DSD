/* Jose Luis Gallego Peña */

#include "calculadora.h"
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>

void op_basicas(char * host, double valor1, char * operador, double valor2){
	calculadora_res * result_1;
	CLIENT * clnt;

	#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	#endif	/* DEBUG */

	/* Usar operación de la calculadora */
	if (!strcmp(operador, "+")){
		result_1 = suma_1(valor1, valor2, clnt);
	}
	else if (!strcmp(operador, "-")){
		result_1 = resta_1(valor1, valor2, clnt);
	}
	else if (!strcmp(operador, "x")){
		result_1 = multiplicacion_1(valor1, valor2, clnt);
	}
	else if (!strcmp(operador, "/")){
		result_1 = division_1(valor1, valor2, clnt);
	}

	if (result_1 == (calculadora_res *) NULL){
		clnt_perror (clnt, "call failed");
	}

	if (result_1->err != 0){
		errno = result_1->err;
		perror("Error en el resultado");
		exit(1);
	}

	// Mostrar resultado de la operación realizada por la calculadora
	printf("\n%lf %s %lf = %lf\n", valor1, operador, valor2, result_1->calculadora_res_u.resultado);

	xdr_free(xdr_calculadora_res, result_1);

	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
}

void op_vectores(char * host, vec vector1, char * operador, vec vector2){
	CLIENT * clnt;
	calculadora_res_vec * result_1;

	#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	#endif	/* DEBUG */

	/* Usar operación de la calculadora */
	if (!strcmp(operador, "+")){
		result_1 = sumavec_1(vector1, vector2, clnt);
	}
	else if (!strcmp(operador, "-")){
		result_1 = restavec_1(vector1, vector2, clnt);
	}
	else if (!strcmp(operador, "x")){
		result_1 = multiplicacionvec_1(vector1, vector2, clnt);
	}

	if (result_1 == (calculadora_res_vec *) NULL){
		clnt_perror (clnt, "call failed");
	}

	if (result_1->err != 0){
		errno = result_1->err;
		perror("Error en el resultado");
		exit(1);
	}

	// Mostrar resultado de la operación realizada por la calculadora
	double * resultado = result_1->calculadora_res_vec_u.resultado.vec_val;
	int tam = result_1->calculadora_res_vec_u.resultado.vec_len;
	printf("\n( ");
	for (int i = 0 ; i < tam ; i++){
		printf("%lf ", vector1.vec_val[i]);
	}
	printf(" ) %s ( ", operador);
	for (int i = 0 ; i < tam ; i++){
		printf("%lf ", vector2.vec_val[i]);
	}
	printf(" ) = ( ");
	for (int i = 0 ; i < tam ; i++){
		printf("%lf ", resultado[i]);
	}
	printf(" )\n");

	xdr_free(xdr_calculadora_res_vec, result_1);

	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
}

void op_vectorescalar(char * host, vec v, double esc){
	CLIENT * clnt;
	calculadora_res_vec * result_1;

	#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	#endif	/* DEBUG */

	/* Usar operación de la calculadora */
	result_1 = multiplicacionvec_esc_1(v, esc, clnt);

	if (result_1->err != 0){
		errno = result_1->err;
		perror("Error en el resultado");
		exit(1);
	}

	// Mostrar resultado de la operación realizada por la calculadora
	double * resultado = result_1->calculadora_res_vec_u.resultado.vec_val;
	int tam = result_1->calculadora_res_vec_u.resultado.vec_len;
	printf("\n( ");
	for (int i = 0 ; i < tam ; i++){
		printf("%lf ", v.vec_val[i]);
	}
	printf(" ) x %lf = ( ", esc);
	for (int i = 0 ; i < tam ; i++){
		printf("%lf ", resultado[i]);
	}
	printf(" )\n");

	xdr_free(xdr_calculadora_res_vec, result_1);

	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
}

void op_polinomio(char * host, polinomio pol){
	CLIENT * clnt;
	calculadora_res * result_1;

	#ifndef	DEBUG
	clnt = clnt_create (host, CALCPROG, CALCVER, "tcp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
	#endif	/* DEBUG */

	/* Usar operación de la calculadora */
	result_1 = polinomio_1(pol, clnt);

	if (result_1->err != 0){
		errno = result_1->err;
		perror("Error en el resultado");
		exit(1);
	}

	// Mostrar resultado de la operación realizada por la calculadora
	printf("%lf\n", result_1->calculadora_res_u.resultado);

	xdr_free(xdr_calculadora_res_vec, result_1);

	#ifndef	DEBUG
	clnt_destroy (clnt);
	#endif	 /* DEBUG */
}

int main (int argc, char *argv[]){
	CLIENT *clnt;
	char *host, *operador;
	double valor1 = 0, valor2 = 0;
	vec vector1, vector2;	// Vectores que se multiplican entre sí
	vec v;	// Vector que se multiplica por un escalar
	double esc;	// Escalar que se multiplica por un vector
	double val;
	polinomio pol;
	int tam = 0, flag_calc = 0;	// Flag que indica si son operaciones básicas (0), con vector (1), de vector por escalar (2) o de polinomios (3)

	/* COMPROBACIONES DE LA ENTRADA */
	// Comprobar que se han introducido el número correcto de argumentos
	// Mostrar operaciones al usuario
	if (argc == 2){
		if (!strcmp(argv[1], "info")){
			printf("CALCULADORA CON SUN RPC - Jose Luis Gallego Peña\n");
			printf("Formato: %s <maquina> <valor1> <operador> <valor2>\n", argv[0]);
			printf("Dependiendo de si valor1 y/o valor2 son números reales o vectores se realizarán unas operaciones u otras. Para introducir un vector, escribir vec en valor1/valor2.\n");
			printf("Para conocer las operaciones reales disponibles: %s op_basicas\n", argv[0]);
			printf("Para conocer las operaciones con vectores disponibles: %s op_vectores\n", argv[0]);
			printf("Para conocer el modo polinomio: %s op_polinomio\n", argv[0]);
			exit(1);
		}

		if (!strcmp(argv[1], "op_basicas")){
			printf("Operaciones básicas con números reales disponibles (v1):\n");
			printf("SUMA: +. Suma dos números reales.\n");
			printf("RESTA: -. Resta dos números reales.\n");
			printf("MULTIPLICACION: x. Multiplica dos números reales.\n");
			printf("DIVISION: /. Divide dos números reales\n");
			exit(1);
		}

		if (!strcmp(argv[1], "op_vectores")){
			printf("Operaciones avanzadas con vectores de números reales disponibles (v2):\n");
			printf("Para introducir un vector en la linea de comandos, escribir vec.\n");
			printf("SUMA DE VECTORES: +. Suma los valores de uno o dos vectores de números reales.\n");
			printf("RESTA DE VECTORES: -. Resta los valores de uno o dos vectores números reales.\n");
			printf("MULTIPLICACION DE VECTORES: x. Multiplica los valores de uno o dos vectores de números reales. Es posible multiplicar todos los valores de un vector por un número escalar real.\n");
			exit(1);
		}

		if (!strcmp(argv[1], "op_polinomio")){
			printf("Operaciones básicas con polinomios de números reales disponibles (v3):\n");
			printf("Para usar el modo polinomio e introducir valores, llamar al programa con la opción polinomio: %s polinomio\n", argv[0]);
			printf("SUMA: +. Suma dos números reales.\n");
			printf("RESTA: -. Resta dos números reales.\n");
			printf("MULTIPLICACION: x. Multiplica dos números reales.\n");
			printf("DIVISION: /. Divide dos números reales\n");
			exit(1);
		}
	}
	else if (argc == 3){
		// Modo polinomio
		if (!strcmp(argv[2], "polinomio")){
			flag_calc = 3;
		}
	}
	else if (argc < 5){
		printf("Error: Formato inválido.\nFormato: %s <maquina> <valor1> <operador> <valor2>\n", argv[0]);
		printf("Modo polinomio: %s polinomio\n", argv[0]);
		printf("Para conocer la información del programa: %s info\n", argv[0]);
		exit(1);
	}

	host = argv[1];

	if (flag_calc != 3){
		operador = argv[3];

		// Introducir vector si se pide
		if (!strcmp(argv[2], "vec") && !strcmp(argv[4], "vec")){	// Vector x Vector
			printf("Introducir tamaño de los vectores: ");
			scanf("%d", &tam);
			flag_calc = 1;
		}
		else if (!strcmp(argv[2], "vec") || !strcmp(argv[4], "vec")){	// Vector x Escalar
			printf("Introducir tamaño del vector: ");
			scanf("%d", &tam);
			flag_calc = 2;
		}

		if (!strcmp(argv[2], "vec")){
			vector1.vec_len = tam;
			vector1.vec_val = malloc(tam * sizeof(double));

			printf("\nIntroducir valores en el vector 1: ");
			for (int i = 0 ; i < tam ; i++){
				scanf("%lf", &val);
				vector1.vec_val[i] = val;
			}

			if (flag_calc == 2)
				v = vector1;
		}
		else{
			valor1 = atof(argv[2]);
			esc = valor1;
		}

		if (!strcmp(argv[4], "vec")){
			vector2.vec_len = tam;
			vector2.vec_val = malloc(tam * sizeof(double));

			printf("\nIntroducir valores en el vector 2: ");
			for (int i = 0 ; i < tam ; i++){
				scanf("%lf", &val);
				vector2.vec_val[i] = val;
			}

			if (flag_calc == 2)
				v = vector2;
		}
		else{
			valor2 = atof(argv[4]);
			esc = valor2;
		}

		// Comprobar que se ha introducido uno de los operadores disponibles
		if (strcmp(operador, "+") && strcmp(operador, "-") && strcmp(operador, "x") && strcmp(operador, "/")){
			printf("Error: el operador %s no está entre los operadores disponibles\n", operador);
			printf("Para conocer las operaciones básicas disponibles: %s op_basicas\n", argv[0]);
			printf("Para conocer las operaciones avanzadas disponibles: %s op_avanzadas\n", argv[0]);
			exit(1);
		}
	}
	else {	// Modo polinomio
		printf("MODO POLINOMIO. Introduzca primero un valor y luego un operador. Para terminar escriba \"=\" después de un valor.\n");

		pol.polinomio_val = malloc(100.0 * sizeof(operacion));
		operador = malloc(sizeof(char));
		operador[0] = '+';

		do {
			pol.polinomio_val[tam].op_izq = *operador;
			scanf("%lf", &val);
			scanf(" %c", &operador[0]);
			pol.polinomio_val[tam].val = val;
			pol.polinomio_val[tam].op_dcha = *operador;

			tam++;
		} while(strcmp(operador, "=") != 0);

		pol.polinomio_len = tam;
	}

	// Llamar a la calculadora
	if (flag_calc == 0)
		op_basicas(host, valor1, operador, valor2);

	if (flag_calc == 1){
		op_vectores(host, vector1, operador, vector2);
		free(vector1.vec_val);
		free(vector2.vec_val);
	}

	if (flag_calc == 2){
		op_vectorescalar(host, v, esc);
		free(v.vec_val);
	}

	if (flag_calc == 3){
		op_polinomio(host, pol);
	}

	exit(0);
}

/*
Compilación:
gcc calculadora_client.c calculadora_clnt.c calculadora_xdr.c -o cliente -lnsl -ltirpc
*/
