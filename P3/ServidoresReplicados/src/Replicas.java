import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Replicas extends UnicastRemoteObject implements Replicas_I {
    private Replicas_I replica; // Interfaz de comunicación servidor-servidor con el resto de servidores
    private Registry registry;
    private int idReplica; // Identificador de esta réplica
    private int numReplicas;    // Número total de réplicas
    private Map<Integer, Integer> registrados = new HashMap<Integer, Integer>(); // Id de usuarios registrados en esta réplica y cantidad donada
    private int subTotal = 0;   // Dinero donado a esta réplica (subtotal de todas las réplicas)

    public Replicas(String host, int idReplica, int numReplicas) throws RemoteException {
        registry = LocateRegistry.getRegistry(host, 1099);
        this.idReplica = idReplica;
        this.numReplicas = numReplicas;
    }

    public int getTotalDonaciones(int idCliente) throws RemoteException, NotBoundException {
        int total = subTotal;

        for (int i = 0 ; i < this.numReplicas ; i++) {
            // Comunicarse con el resto de réplicas
            if (i != this.idReplica) {
                replica = (Replicas_I) registry.lookup("Replica" + i);
                total += replica.getSubTotalDonaciones();
            }
        }

        return total;
    }

    public int getSubTotalDonaciones() throws RemoteException {
        return subTotal;
    }

    public int registroCliente(int id) throws RemoteException, NotBoundException {
        int replicaRegistro = idReplica;
        int numRegistrados;
        int minNumRegistrados = getNumRegistrados();
        boolean registrado = false;

        for (int i = 0 ; i < this.numReplicas && !registrado ; i++) {
            // Comunicarse con el resto de réplicas
            if (i != this.idReplica) {
                replica = (Replicas_I) registry.lookup("Replica" + i);

                // Comprobar si el cliente ya estaba registrado antes
                registrado = replica.estaRegistrado(id);

                // Obtener réplica con menor número de registrados
                numRegistrados = replica.getNumRegistrados();
                if (numRegistrados < minNumRegistrados) {
                    replicaRegistro = numRegistrados;
                    minNumRegistrados = numRegistrados;
                }
            }
        }

        // Registrar cliente en réplica con menor número de registrados
        if (!registrado) {
            if (replicaRegistro != idReplica) {
                replica = (Replicas_I) registry.lookup("Replica" + replicaRegistro);
                replica.registrar(id);
            }
            // Registrar cliente en esta réplica (con la que se comunicó inicialmente)
            else {
                registrar(id);
            }
        }

        return replicaRegistro;
    }

    public synchronized void registrar(int idCliente) throws RemoteException {
        registrados.put(idCliente, 0);
    }

    public boolean estaRegistrado(int idCliente) throws RemoteException {
        return registrados.containsKey(idCliente);
    }

    public int getNumRegistrados() throws RemoteException {
        return registrados.size();
    }

    public synchronized void donar(int cantidad, int idCliente) throws RemoteException {
        registrados.put(idCliente, 0);
        subTotal += cantidad;
    }
}
