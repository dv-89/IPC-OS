import java.io.IOException;
import java.io.DataInputStream;
import java.net.Socket;

public class Consumer {
    public static void main(String[] args) {
        try {
            // Create a client socket to connect to the producer
            Socket socket = new Socket("localhost", 12345);
            System.out.println("Connected to producer.");

            // Create a data input stream to receive data
            DataInputStream inputStream = new DataInputStream(socket.getInputStream());


            // Receive and process random integers
            for (int i = 0; i < 100; i++) {
                int receivedInt = inputStream.readInt();
                System.out.println("Received: " + receivedInt);
            }

            // Close the socket
            inputStream.close();
            socket.close();
            System.out.println("Consumer finished.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
