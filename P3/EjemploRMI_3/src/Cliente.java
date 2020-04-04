import java.net.MalformedURLException;
import java.rmi.registry.LocateRegistry;
import java.rmi.*;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Cliente {
    public static void main(String[] args) {
        if (System.getSecurityManager() == null) {
            System.setSecurityManager(new SecurityManager());
        }

        String host = "";
        int c = 0;
        Scanner keyboard = new Scanner(System.in);

        System.out.println("Introduce el servidor: ");
        host = keyboard.nextLine();
        System.out.println("Introduce el primer valor del contador: ");
        c = Integer.parseInt(keyboard.nextLine());

        try {
            Registry registry = LocateRegistry.getRegistry(host, 1099);
            System.out.println("Buscando el objeto remoto");
            icontador local = (icontador) registry.lookup("micontador");
            System.out.println("Invocando el método remoto");

            System.out.println("Inicializando contador");
            local.sumar(c);

            // Obtiene hora de comienzo
            long horacomienzo = System.currentTimeMillis();

            // Incrementa 1000 veces
            System.out.println("Incrementando...");
            for (int i = 0 ; i < 1000 ; i++) {
                local.incrementar();
            }

            // Obtiene hora final
            long horafin = System.currentTimeMillis();
            // Cálculo tiempo transcurrido
            float transcurrido = ((horafin - horacomienzo)/1000f);

            System.out.println("Media de las RMI realizadas = " + transcurrido + " msegs");
            System.out.println("RMI realizadas = " + local.sumar());
        } catch (Exception e) {
            System.err.println("Error en el cliente: ");
            e.printStackTrace();
        }
    }
}
