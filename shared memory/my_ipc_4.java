import java.util.concurrent.Semaphore;

class my_ipc_4 {
    private static final int BUFFER_SIZE = 10;
    private int[] buffer = new int[BUFFER_SIZE];
    private int in = 0; // points to next free element
    private int out = 0; // points to the first full buffer element
    private int count = 0; // counts the number of data items in the buffer
    private int totalGenerated = 0;

    private Semaphore semEmpty = new Semaphore(BUFFER_SIZE);
    private Semaphore semFull = new Semaphore(0);
    private Semaphore mutex = new Semaphore(1);

    public void producerProcess() {
        while (totalGenerated <= 100) {
            int x = (int) (Math.random() * 100);
            try {
                semEmpty.acquire();
                mutex.acquire();

                buffer[in] = x;
                in = (in + 1) % BUFFER_SIZE;
                count++;

                mutex.release();
                semFull.release();

                totalGenerated++;
                System.out.println("Generated: " + x);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void consumerProcess() {
        while (totalGenerated <= 100) {
            while (count == 0) {
                // Buffer is empty, so do nothing and wait
            }

            try {
                semFull.acquire();
                mutex.acquire();

                int y = buffer[out];
                out = (out + 1) % BUFFER_SIZE;
                count--;

                mutex.release();
                semEmpty.release();

                System.out.println("Consumed: " + y);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        my_ipc_4 obj = new my_ipc_4();

        Thread producerThread = new Thread(obj::producerProcess);
        Thread consumerThread = new Thread(obj::consumerProcess);

        producerThread.start();
        consumerThread.start();

        try {
            producerThread.join();
            consumerThread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
