import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Servidor {
    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        try {
            Registry registry = LocateRegistry.createRegistry(1099);

            // Objeto remoto
            Ejemplo ejemplo = new Ejemplo();
            Naming.rebind("ejemplo", ejemplo);

            System.out.println("Servidor listo");
        } catch (Exception e) {
            System.err.println("Excepción en el servidor: ");
            e.printStackTrace();
        }
    }
}
