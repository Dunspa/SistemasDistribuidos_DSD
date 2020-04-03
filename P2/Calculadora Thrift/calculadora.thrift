struct operacion{
1: required double val;
2: required string op_izq;
3: required string op_dcha;
}

service Calculadora{
   void ping(),
   double suma(1: double num1, 2: double num2),
   double resta(1: double num1, 2: double num2),
   double multiplicacion(1: double num1, 2: double num2),
   double division(1: double num1, 2: double num2),
   list<double> sumavec(1: list<double> vec1, 2: list<double> vec2),
   list<double> restavec(1: list<double> vec1, 2: list<double> vec2),
   list<double> multiplicacionvec(1: list<double> vec1, 2: list<double> vec2),
   list<double> multiplicacionvec_esc(1: list<double> vec, 2: double num),
   double polinomio(1: list<operacion> polinomio),
}
