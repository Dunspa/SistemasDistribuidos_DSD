import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Server {
    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        try {
            Registry registry = LocateRegistry.createRegistry(1099);
            Example example = new Example();
            Naming.rebind("example", example);
            System.out.println("Server RemoteException | MalformedURLExceptiondor ready");
        } catch (Exception e) {
            System.err.println("Example exception: ");
            e.printStackTrace();
        }
    }
}