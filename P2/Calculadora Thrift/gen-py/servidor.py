import glob
import sys
import numpy as np

from calculadora import Calculadora
# from calculadora . ttypes import Operation
# Lo de ttypes es si hubieramos anadido tipos en el fichero . thrift

# hay que instalar antes el paquete thrift de python
# ( no confundir con el compilador thrift )
from thrift.transport import TSocket
from thrift.transport import TTransport
from thrift.protocol import TBinaryProtocol
from thrift.server import TServer

import logging
logging.basicConfig(level = logging.DEBUG)

class CalculadoraHandler:
    def __init__(self):
        self.log = {}

    def ping(self):
        print('Llamada al servidor')

    def suma(self, n1, n2):
        print('Sumando ' + str(n1) + ' con ' + str(n2))
        return n1 + n2

    def resta(self, n1, n2):
        print('Restando ' + str(n1) + ' con ' + str(n2))
        return n1 - n2

    def multiplicacion(self, n1, n2):
        print('Multiplicando ' + str(n1) + ' con ' + str(n2))
        return n1 * n2

    def division(self, n1, n2):
        print('Dividiendo ' + str(n1) + ' con ' + str(n2))
        return n1 / n2

    def sumavec(self, v1, v2):
        print('Sumando vectores ' + str(v1) + ' con ' + str(v2))
        v1 = np.array(v1)
        v2 = np.array(v2)
        return v1 + v2

    def restavec(self, v1, v2):
        print('Restando vectores ' + str(v1) + ' con ' + str(v2))
        v1 = np.array(v1)
        v2 = np.array(v2)
        return v1 - v2

    def multiplicacionvec(self, v1, v2):
        print('Multiplicando vectores ' + str(v1) + ' con ' + str(v2))
        v1 = np.array(v1)
        v2 = np.array(v2)
        return v1 * v2

    def multiplicacionvec_esc(self, v, esc):
        print('Multiplicando vector ' + str(v) + ' con escalar ' + str(esc))
        v = np.array(v)
        return v * esc

    def polinomio(self, pol):
        print('Operando polinomio')
        result_parcial = 0
        result_mult = 1
        result_div = 1

        for i in range(0, len(pol)):
            op_izq = pol[i].op_izq
            op_dcha = pol[i].op_dcha

            if op_izq == '+':
                if op_dcha != 'x' and op_dcha != '/':
                    result_parcial += pol[i].val

                result_mult = pol[i].val
                result_div = pol[i].val

            if op_izq == '-':
                if op_dcha != 'x' and op_dcha != '/':
                    result_parcial -= pol[i].val

                result_mult = -pol[i].val
                result_div = -pol[i].val

            if op_izq == 'x' and op_dcha != 'x':
                if pol[i-1].op_izq == '/':
                    if result_mult != 1:
                        result_mult = pol[i].val

                    result_mult *= result_div
                    result_parcial = result_mult
                else:
                    result_parcial += result_mult
            else:
                if op_dcha == 'x':
                    result_mult *= pol[i+1].val

            if op_izq == '/' and op_dcha != '/':
                if pol[i-1].op_izq == 'x':
                    if result_div != 1:
                        result_div = pol[i].val

                    result_div = result_mult / result_div
                    result_parcial = result_div
                else:
                    result_parcial += result_div
            else:
                if op_dcha == '/':
                    result_div /= pol[i+1].val

        return result_parcial


if __name__ == '__main__':
    handler = CalculadoraHandler()
    processor = Calculadora.Processor(handler)
    transport = TSocket.TServerSocket(host = '127.0.0.1', port = 9090)
    tfactory = TTransport.TBufferedTransportFactory()
    pfactory = TBinaryProtocol.TBinaryProtocolFactory()

    server = TServer.TSimpleServer(processor, transport, tfactory, pfactory)
    print('Iniciando servidor ...')
    server.serve()
    print ('Fin.')
