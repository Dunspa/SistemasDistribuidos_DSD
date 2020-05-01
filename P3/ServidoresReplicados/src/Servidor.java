import java.rmi.Naming;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Servidor {
    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        try {
            Registry registry = LocateRegistry.createRegistry(1099);
            int numReplicas = 5;

            // Instanciar n réplicas
            for (int i = 0 ; i < numReplicas ; i++) {
                String remoteObjectName = "Replica" + i;

                Replicas replica = new Replicas("localhost", i, numReplicas);    // Servidor-servidor
                Naming.rebind(remoteObjectName, replica);

                Donaciones donaciones = new Donaciones("localhost", replica); // Cliente-servidor
                Naming.rebind(remoteObjectName+"Donaciones", donaciones);

                System.out.println("Replica" + i + " lista");
            }

            System.out.println("Todas las réplicas están listas");
        } catch (Exception e) {
            System.err.println("Replica exception:");
            e.printStackTrace();
        }
    }
}
