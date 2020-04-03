import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Cliente implements Runnable {
    public String servidor;

    public Cliente(String s) {
        servidor = s;
    }
    
    public void run() {
        try {
            Registry registry = LocateRegistry.getRegistry(servidor, 1099);
            System.out.println("Buscando el objeto remoto");
            Ejemplo_I local = (Ejemplo_I) registry.lookup("ejemplo");   // Objeto remoto llamado ejemplo
            System.out.println("Invocando el método remoto");
            local.escribir_mensaje(Thread.currentThread().getName());
        } catch (Exception e) {
            System.err.println("Error en el cliente: ");
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        String host = "";
        int n_hebras;
        Scanner keyboard = new Scanner(System.in);

        System.out.println("Introduce el servidor: ");
        host = keyboard.nextLine();
        System.out.println("Introduce el número de hebras: ");
        n_hebras = Integer.parseInt(keyboard.nextLine());

        if (System.getSecurityManager() == null){
            System.setSecurityManager(new SecurityManager());
        }

        // Array con todos los clientes (hebras)
        Cliente[] v_clientes = new Cliente[n_hebras];
        Thread[] v_hebras = new Thread[n_hebras];

        for (int i = 0 ; i < n_hebras ; i++){
            // A cada hebra le pasamos el nombre del servidor
            v_clientes[i] = new Cliente(host);
            v_hebras[i] = new Thread(v_clientes[i], "Cliente " + i);
            v_hebras[i].start();
        }
    }
}
