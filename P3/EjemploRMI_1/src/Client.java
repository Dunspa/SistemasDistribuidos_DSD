import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        String host = "";
        int processId;
        Scanner keyboard = new Scanner(System.in);

        System.out.println("Enter server IP or server name: ");
        host = keyboard.nextLine();
        System.out.println("Enter process ID: ");
        processId = keyboard.nextInt();

        if (System.getSecurityManager() == null){
            System.setSecurityManager(new SecurityManager());
        }

        try {
            Registry registry = LocateRegistry.getRegistry(host, 1099);
            System.out.println("Searching for the remote object");
            Example_I localInstance = (Example_I) registry.lookup("example");
            System.out.println("Invoking the remote object");
            localInstance.writeMessage(processId);
        } catch (Exception e) {
            System.err.println("Example exception: ");
            e.printStackTrace();
        }
    }
}