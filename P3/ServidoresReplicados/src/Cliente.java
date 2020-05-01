import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;

public class Cliente implements Runnable {
    private String host;
    private String replica;

    public Cliente(String host, String replica) {
        this.host = host;
        this.replica = replica;
    }

    public static void main(String args[]) {
        String host = "localhost";
        String replica = "";
        Scanner s = new Scanner(System.in);
        ArrayList<Cliente> v_clientes = new ArrayList<>();
        ArrayList<Thread> v_hebras = new ArrayList<>();
        int numClientes = 0;
        int totalClientes = 0;

        do {
            System.out.print("Elige la réplica con la que quieres comunicarte (0...4): ");
            replica = s.nextLine();
            System.out.println("Introduce el número de clientes que se conectarán a la réplica: ");
            numClientes = Integer.parseInt(s.nextLine());

            if (System.getSecurityManager() == null) {
                System.setSecurityManager(new SecurityManager());
            }

            // Array con todos los clientes (hebras)
            for (int i = 0 ; i < numClientes; i++) {
                v_clientes.add(new Cliente(host, replica));
            }

            System.out.println("¿Conectar a otra réplica? (Si/No)");
            totalClientes += numClientes;
        } while (s.nextLine().toUpperCase().equals("SI"));

        // Lanzar hebras
        for (int i = 0 ; i < totalClientes ; i++) {
            v_hebras.add(new Thread(v_clientes.get(i), Integer.toString(i)));
            v_hebras.get(i).start();
        }
    }

    @Override
    public void run() {
        try {
            Registry registry = LocateRegistry.getRegistry(this.host, 1099);
            int donacion = 0;

            // Cliente desea registrarse
            System.out.println("Efectuando registro del Cliente" + Thread.currentThread().getName() + " en Replica" + this.replica + "...");
            Donaciones_I local = (Donaciones_I) registry.lookup("Replica" + this.replica + "Donaciones");
            this.replica = Integer.toString(local.registroCliente(Integer.parseInt(Thread.currentThread().getName())));
            System.out.println("Cliente" + Thread.currentThread().getName() + " registrado en Replica" + this.replica);

            // Cliente dona en el servidor donde ha sido registrado
            donacion = new Random().nextInt(100);
            System.out.println("Cliente" + Thread.currentThread().getName() + " dona " + donacion + " euros en Replica" + this.replica + "...");
            local = (Donaciones_I) registry.lookup("Replica" + this.replica + "Donaciones");
            local.donar(donacion, Integer.parseInt(Thread.currentThread().getName()));
            System.out.println("Donación del Cliente" + Thread.currentThread().getName() + " completada");

            // Mostrar total de donaciones
            System.out.println("(Cliente" + Thread.currentThread().getName() + ") Total donaciones: " + local.getTotalDonaciones(Integer.parseInt(Thread.currentThread().getName())));
        } catch (Exception e) {
            System.err.println("Error en el Cliente" + Thread.currentThread().getName());
            e.printStackTrace();
        }
    }
}