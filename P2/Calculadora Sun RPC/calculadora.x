/* Jose Luis Gallego Peña */

typedef double vec<>;	/* vector de números */
typedef struct operacion polinomio<>; /* Polinomio (vector de operaciones) */

/* Una operación es parte de un polinomio está compuesta por un
 * número y sus operadores a los lados */
struct operacion{
	double val;
	char op_izq;	/* Operador a su izquierda */
	char op_dcha;	/* Operador a su derecha */
};

/* Uniones para discriminar entre llamadas con exito y llamadas con errores */

union calculadora_res switch(int err) {
	case 0:
		double resultado; /* sin error: resultado de la operación básica */
	default:
		void; /* con error: nada */
};

union calculadora_res_vec switch(int err) {
	case 0:
		vec resultado; /* sin error: resultado de operación con vectores */
	default:
		void; /* con error: nada */
};

program CALCPROG {
	version CALCVER {
		calculadora_res SUMA(double, double) = 1;
		calculadora_res RESTA(double, double) = 2;
		calculadora_res MULTIPLICACION(double, double) = 3;
		calculadora_res DIVISION(double, double) = 4;
		calculadora_res_vec SUMAVEC(vec, vec) = 5;
		calculadora_res_vec RESTAVEC(vec, vec) = 6;
		calculadora_res_vec MULTIPLICACIONVEC(vec, vec) = 7;
		calculadora_res_vec MULTIPLICACIONVEC_ESC(vec, double) = 8;
		calculadora_res POLINOMIO(polinomio) = 9;
	} = 1;
} = 0x20000001;
