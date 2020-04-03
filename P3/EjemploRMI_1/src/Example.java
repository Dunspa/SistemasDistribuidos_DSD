import java.lang.Thread;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class Example extends UnicastRemoteObject implements Example_I {
    protected Example() throws RemoteException {

    }

    public void writeMessage(int processId) throws RemoteException {
        System.out.println("Process request received: " + processId);

        if (processId == 0) {
            try {
                System.out.println("We start sleeping");
                Thread.sleep(5000);
                System.out.println("We finished sleeping");
            } catch (Exception e) {
                System.err.println("Example exception: ");
                e.printStackTrace();
            }
        }

        System.out.println("\nThread " + processId);
    }
}