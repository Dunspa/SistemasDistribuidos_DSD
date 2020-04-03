/* Jose Luis Gallego Peña */

#include "calculadora.h"

calculadora_res * suma_1_svc(double arg1, double arg2, struct svc_req *rqstp){
	static calculadora_res result;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	// Operación suma de la calculadora
	result.calculadora_res_u.resultado = arg1 + arg2;

	return &result;
}

calculadora_res * resta_1_svc(double arg1, double arg2, struct svc_req *rqstp){
	static calculadora_res result;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	// Operación resta de la calculadora
	result.calculadora_res_u.resultado = arg1 - arg2;

	return &result;
}

calculadora_res * multiplicacion_1_svc(double arg1, double arg2, struct svc_req *rqstp){
	static calculadora_res result;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	// Operación multiplicación de la calculadora
	result.calculadora_res_u.resultado = arg1 * arg2;

	return &result;
}

calculadora_res * division_1_svc(double arg1, double arg2, struct svc_req *rqstp){
	static calculadora_res result;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	// Operación division de la calculadora
	result.calculadora_res_u.resultado = arg1 / arg2;

	return &result;
}

calculadora_res_vec * sumavec_1_svc(vec arg1, vec arg2, struct svc_req *rqstp){
	static calculadora_res_vec result;
	double * vec1 = arg1.vec_val;
	double * vec2 = arg2.vec_val;
	int tam = arg1.vec_len;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	result.calculadora_res_vec_u.resultado.vec_val = malloc(tam * sizeof(double));
	result.calculadora_res_vec_u.resultado.vec_len = tam;

	// Operación suma de vectores de la calculadora
	for (int i = 0 ; i < tam ; i++){
		result.calculadora_res_vec_u.resultado.vec_val[i] = vec1[i] + vec2[i];
	}

	return &result;
}

calculadora_res_vec * restavec_1_svc(vec arg1, vec arg2, struct svc_req *rqstp){
	static calculadora_res_vec result;
	double * vec1 = arg1.vec_val;
	double * vec2 = arg2.vec_val;
	int tam = arg1.vec_len;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	result.calculadora_res_vec_u.resultado.vec_val = malloc(tam * sizeof(double));
	result.calculadora_res_vec_u.resultado.vec_len = tam;

	// Operación resta de vectores de la calculadora
	for (int i = 0 ; i < tam ; i++){
		result.calculadora_res_vec_u.resultado.vec_val[i] = vec1[i] - vec2[i];
	}

	return &result;
}

calculadora_res_vec * multiplicacionvec_1_svc(vec arg1, vec arg2, struct svc_req *rqstp){
	static calculadora_res_vec result;
	double * vec1 = arg1.vec_val;
	double * vec2 = arg2.vec_val;
	int tam = arg1.vec_len;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	result.calculadora_res_vec_u.resultado.vec_val = malloc(tam * sizeof(double));
	result.calculadora_res_vec_u.resultado.vec_len = tam;

	// Operación multiplicación de vectores de la calculadora
	for (int i = 0 ; i < tam ; i++){
		result.calculadora_res_vec_u.resultado.vec_val[i] = vec1[i] * vec2[i];
	}

	return &result;
}

calculadora_res_vec * multiplicacionvec_esc_1_svc(vec arg1, double arg2, struct svc_req *rqstp){
	static calculadora_res_vec result;
	double esc = arg2;
	double * v = arg1.vec_val;
	int tam = arg1.vec_len;

	// Libera la memoria que se asigno * (para el resultado) en una ejecucion previa del servidor
	xdr_free(xdr_calculadora_res, &result);

	result.calculadora_res_vec_u.resultado.vec_val = malloc(tam * sizeof(double));
	result.calculadora_res_vec_u.resultado.vec_len = tam;

	// Operación multiplicación de vector por escalar de la calculadora
	for (int i = 0 ; i < tam ; i++){
		result.calculadora_res_vec_u.resultado.vec_val[i] = v[i] * esc;
	}

	return &result;
}

calculadora_res * polinomio_1_svc(polinomio pol, struct svc_req *rqstp){
	static calculadora_res result;
	double result_parcial = 0;
	double result_mult = 1, result_div = 1;
	char op_izq, op_dcha;
	result.calculadora_res_u.resultado = 0;

	for (int i = 0 ; i < pol.polinomio_len ; i++){
		op_izq = pol.polinomio_val[i].op_izq;
		op_dcha = pol.polinomio_val[i].op_dcha;

		if (op_izq == '+'){
			if (op_dcha != 'x' && op_dcha != '/'){
				result_parcial += pol.polinomio_val[i].val;
			}

			result_mult = pol.polinomio_val[i].val;
			result_div = pol.polinomio_val[i].val;
		}

		if (op_izq == '-'){
			if (op_dcha != 'x' && op_dcha != '/'){
				result_parcial -= pol.polinomio_val[i].val;
			}

			result_mult = -pol.polinomio_val[i].val;
			result_div = -pol.polinomio_val[i].val;
		}

		// Final de la cadena de multiplicación
		if (op_izq == 'x' && op_dcha != 'x'){
			if (pol.polinomio_val[i-1].op_izq == '/'){
				if (result_mult != 1){
					result_mult = pol.polinomio_val[i].val;
				}

				result_mult *= result_div;
				result_parcial = result_mult;
			}
			else{
				result_parcial += result_mult;
			}
		}
		else if (op_dcha == 'x'){	// Cadena de multiplicación
			result_mult *= pol.polinomio_val[i+1].val;
		}

		// Final de la cadena de división
		if (op_izq == '/' && op_dcha != '/'){
			if (pol.polinomio_val[i-1].op_izq == 'x'){
				if (result_div != 1){
					result_div = pol.polinomio_val[i].val;
				}

				result_div = result_mult / result_div;
				result_parcial = result_div;
			}
			else{
				result_parcial += result_div;
			}
		}
		else if (op_dcha == '/'){ // Cadena de división
			result_div /= pol.polinomio_val[i+1].val;
		}
	}

	result.calculadora_res_u.resultado += result_parcial;

	return &result;
}

/*
Compilación:
gcc calculadora_server.c calculadora_svc.c calculadora_xdr.c -o servidor -lnsl -ltirpc
*/
