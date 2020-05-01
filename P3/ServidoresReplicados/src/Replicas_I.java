import java.rmi.NotBoundException;
import java.rmi.Remote;
import java.rmi.RemoteException;

// Interfaz de comunicación servidor-servidor
public interface Replicas_I extends Remote {
    public int getTotalDonaciones(int idCliente) throws RemoteException, NotBoundException;

    public int getSubTotalDonaciones() throws RemoteException;

    public int registroCliente(int idCliente) throws RemoteException, NotBoundException;

    public void registrar(int idCliente) throws RemoteException;

    public boolean estaRegistrado(int idCliente) throws RemoteException;

    public int getNumRegistrados() throws RemoteException;

    public void donar(int cantidad, int idCliente) throws RemoteException;
}
