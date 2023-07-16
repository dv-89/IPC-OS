import java.io.IOException;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Random;

public class Producer {
    public static void main(String[] args) {
        try {
            // Create a server socket
            ServerSocket serverSocket = new ServerSocket(12345);
            System.out.println("Producer started. Listening on port 12345...");

            // Accept a client connection
            Socket clientSocket = serverSocket.accept();
            System.out.println("Consumer connected.");

            // Create an output stream to send data
            DataOutputStream  outputStream = new DataOutputStream(clientSocket.getOutputStream());

            // Generate and send random integers
            Random random = new Random();
            for (int i = 0; i < 100; i++) {
                int randomInt = random.nextInt(1000);
                outputStream.writeInt(randomInt);
                System.out.println("Sent: " + randomInt);
            }

            // Close the sockets
            outputStream.close();
            clientSocket.close();
            serverSocket.close();
            System.out.println("Producer finished.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
