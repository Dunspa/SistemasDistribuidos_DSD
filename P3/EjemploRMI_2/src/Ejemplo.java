import java.lang.Thread;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class Ejemplo extends UnicastRemoteObject implements Ejemplo_I {
    protected Ejemplo() throws RemoteException {

    }

    public synchronized void escribir_mensaje(String mensaje) throws RemoteException {
        System.out.println("Entra hebra: " + mensaje);

        // Buscamos los procesos
        if (mensaje.endsWith("0")) {
            try {
                System.out.println("Empezamos a dormir");
                Thread.sleep(5000);
                System.out.println("Terminamos de dormir");
            } catch (Exception e) {
                System.err.println("Excepción método objeto remoto: ");
                e.printStackTrace();
            }
        }

        System.out.println("Sale hebra: " + mensaje);
    }
}