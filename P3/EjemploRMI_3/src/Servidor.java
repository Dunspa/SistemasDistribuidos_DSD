import java.net.MalformedURLException;
import java.rmi.*;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.*;

public class Servidor {
    public static void main(String[] args) {
        if (System.getSecurityManager() == null){
            System.setSecurityManager(new SecurityManager());
        }

        try {
            Registry registry = LocateRegistry.createRegistry(1099);
            contador micontador = new contador();
            Naming.rebind("micontador", micontador);
            System.out.println("Servidor funcionando");
        } catch (Exception e) {
            System.err.println("Error en el servidor: ");
            e.printStackTrace();
        }
    }
}