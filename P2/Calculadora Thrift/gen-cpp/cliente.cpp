#include <iostream>
#include <vector>

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

#include "Calculadora.h"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

int main(int argc, char * argv[]){
   double resultado;
   vector<double> resultado_vec;
	char *operador;
	double valor1 = 0, valor2 = 0;
	vector<double> vector1, vector2;	// Vectores que se multiplican entre sí
   vector<double> v;	// Vector que se multiplica por un escalar
	double esc;	// Escalar que se multiplica por un vector
	double val;
   operacion op;
	vector<operacion> pol;
	int tam = 0, flag_calc = 0;	// Flag que indica si son operaciones básicas (0), con vector (1), de vector por escalar (2) o de polinomios (3)

   /* COMPROBACIONES DE LA ENTRADA */
	// Comprobar que se han introducido el número correcto de argumentos
	// Mostrar operaciones al usuario
	if (argc == 2){
		if (!strcmp(argv[1], "info")){
			printf("CALCULADORA CON SUN RPC - Jose Luis Gallego Peña\n");
			printf("Formato: %s <valor1> <operador> <valor2>\n", argv[0]);
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

      // Modo polinomio
		if (!strcmp(argv[1], "polinomio")){
			flag_calc = 3;
		}
      else{
         cout << "El parámetro " << argv[1] << " no es un parámetro válido\n";
         cout << "Para conocer la información del programa: " << argv[0] << " info\n";
         exit(1);
      }
	}
	else if (argc < 4){
		printf("Error: Formato inválido.\nFormato: %s <valor1> <operador> <valor2>\n", argv[0]);
		printf("Modo polinomio: %s polinomio\n", argv[0]);
		printf("Para conocer la información del programa: %s info\n", argv[0]);
		exit(1);
	}
   
   // Crear socket
   std::shared_ptr<TTransport> socket(new TSocket("127.0.0.1", 9090));

   // Buffering
   std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));

   // Crear protocolo de transporte
   std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

   // Crear cliente que use el protocolo de transporte
   CalculadoraClient client(protocol);

   if (flag_calc != 3){
		operador = argv[2];

		// Introducir vector si se pide
		if (!strcmp(argv[1], "vec") && !strcmp(argv[3], "vec")){	// Vector x Vector
			cout << "Introducir tamaño de los vectores: ";
			cin >> tam;
			flag_calc = 1;
		}
		else if (!strcmp(argv[1], "vec") || !strcmp(argv[3], "vec")){	// Vector x Escalar
			cout << "Introducir tamaño del vector: ";
			cin >> tam;
			flag_calc = 2;
		}

		if (!strcmp(argv[1], "vec")){
			cout << "\nIntroducir valores en el vector 1: ";
			for (int i = 0 ; i < tam ; i++){
				cin >> val;
				vector1.push_back(val);
			}

			if (flag_calc == 2)
				v = vector1;
		}
		else{
			valor1 = atof(argv[1]);
			esc = valor1;
		}

		if (!strcmp(argv[3], "vec")){
			cout << "\nIntroducir valores en el vector 2: ";
			for (int i = 0 ; i < tam ; i++){
				cin >> val;
				vector2.push_back(val);
			}

			if (flag_calc == 2)
				v = vector2;
		}
		else{
			valor2 = atof(argv[3]);
			esc = valor2;
		}

		// Comprobar que se ha introducido uno de los operadores disponibles
		if (strcmp(operador, "+") && strcmp(operador, "-") && strcmp(operador, "x") && strcmp(operador, "/")){
			cout << "Error: el operador " << operador << " no está entre los operadores disponibles\n";
			cout << "Para conocer las operaciones básicas disponibles: " << argv[0] << " op_basicas\n";
			cout << "Para conocer las operaciones avanzadas con vectores disponibles: " << argv[0] << " op_vectores\n";
         cout << "Para conocer las operaciones con polinomios disponibles: " << argv[0] << " op_polinomio\n";
			exit(1);
		}
	}
	else {	// Modo polinomio
		cout << "MODO POLINOMIO. Introduzca primero un valor y luego un operador. Para terminar escriba \"=\" después de un valor.\n";

		operador = new char[sizeof(char)];
		operador[0] = '+';

		do {
         op.op_izq = *operador;
			cin >> val;
			cin >> operador[0];
         op.val = val;
         op.op_dcha = *operador;
         pol.push_back(op);
      } while(strcmp(operador, "=") != 0);
	}

   try {
      // Abrir conexión
      transport->open();

      cout << "Hacemos ping al server" << endl;
      client.ping();

      // Llamar a la calculadora
   	if (flag_calc == 0){
         if (!strcmp(operador, "+")){
      		resultado = client.suma(valor1, valor2);
      	}
      	else if (!strcmp(operador, "-")){
      		resultado = client.resta(valor1, valor2);
      	}
      	else if (!strcmp(operador, "x")){
      		resultado = client.multiplicacion(valor1, valor2);
      	}
      	else if (!strcmp(operador, "/")){
      		resultado = client.division(valor1, valor2);
      	}

         // Mostrar resultado de la operación realizada por la calculadora
      	cout << valor1 << " " << operador << " " << valor2 << " = " << resultado << "\n";
      }

   	if (flag_calc == 1){
         // Llamar a la calculadora
         if (!strcmp(operador, "+")){
            client.sumavec(resultado_vec, vector1, vector2);
         }
         else if (!strcmp(operador, "-")){
            client.restavec(resultado_vec, vector1, vector2);
         }
         else if (!strcmp(operador, "x")){
            client.multiplicacionvec(resultado_vec, vector1, vector2);
         }

         // Mostrar resultado de la operación realizada por la calculadora
      	int tam = vector1.size();
      	cout << "\n( ";
      	for (int i = 0 ; i < tam ; i++){
      		cout << vector1[i] << " ";
      	}
      	cout << ") " << operador << " ( ";
      	for (int i = 0 ; i < tam ; i++){
      		cout << vector2[i] << " ";
      	}
      	cout << ") = ( ";
      	for (int i = 0 ; i < tam ; i++){
      		cout << resultado_vec[i] << " ";
      	}
      	cout << ")\n";
   	}

   	if (flag_calc == 2){
         // Llamar a la calculadora
         client.multiplicacionvec_esc(resultado_vec, v, esc);

         // Mostrar resultado de la operación realizada por la calculadora
         int tam = vector1.size();
      	cout << "\n( ";
      	for (int i = 0 ; i < tam ; i++){
      		cout << v[i] << " ";
      	}
      	cout << ") x " << esc << " = ( ";
      	for (int i = 0 ; i < tam ; i++){
      		cout << resultado_vec[i] << " ";
      	}
      	cout << ")\n";
   	}

   	if (flag_calc == 3){
         // Llamar a la calculadora
         resultado = client.polinomio(pol);

         // Mostrar resultado de la operación realizada por la calculadora
         cout << "Resultado polinomio: " << resultado << endl;
   	}

      cout << "Cierre de la conexión con el servidor." << endl;

      // Cerrar conexión
      transport->close();
   } catch (TException& tx) {
      cout << "ERROR: " << tx.what() << endl;
   }
}
