import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Random;

public class Donaciones extends UnicastRemoteObject implements Donaciones_I {
    Registry registry;
    Replicas replica; // Replica de este objeto de donación

    public Donaciones(String host, Replicas replica) throws RemoteException {
        //registry = LocateRegistry.getRegistry(host, 1099);
        this.replica = replica;
    }

    public int getTotalDonaciones(int idCliente) throws RemoteException, NotBoundException {
        return replica.getTotalDonaciones(idCliente);
    }

    public int registroCliente(int idCliente) throws RemoteException, NotBoundException {
        return replica.registroCliente(idCliente);
    }

    public void donar(int cantidad, int idCliente) throws RemoteException {
        replica.donar(cantidad, idCliente);
    }
}