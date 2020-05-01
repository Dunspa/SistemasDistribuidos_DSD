import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;

// Interfaz de comunicación cliente-servidor
public interface Donaciones_I extends Remote {
    public int getTotalDonaciones(int idCliente) throws RemoteException, NotBoundException;

    public int registroCliente(int idCliente) throws RemoteException, NotBoundException;

    public void donar(int cantidad, int idCliente) throws RemoteException;
}
