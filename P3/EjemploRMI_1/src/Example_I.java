import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Example_I extends Remote {
    public void writeMessage(int processId) throws RemoteException;
}