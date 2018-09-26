
import java.util.*;

public class StressGC {

    public static final int NUMBER_OF_STRESS_THREADS = 16;
    public static final int MAX_ARRAY_SIZE = 2048;
    public static final int MAX_OBJECTS_NUMBER = 8192;
    public static final int ITERATIONS = 2048;
//    static Random rand = new Random();


    public int stressGC() {

        int passed = 0;
        Thread[] threads = new Thread[NUMBER_OF_STRESS_THREADS];

        for (int i = 0; i < threads.length; i++) {
            threads[i] = new CreateObjectsThread();
            threads[i].start();

        }

        for (int i = 0; i < threads.length; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException ie) {
                passed++;
            }
        }

        return passed;
    }

    public class CreateObjectsThread extends Thread {



        public void run() {
            for (int i = 0; i < ITERATIONS ; i++ ) {
                ArrayList<Object> arr = new ArrayList<Object>();
                try {
                    arr.add(new String[MAX_ARRAY_SIZE]);
                    arr.add(new Object[MAX_ARRAY_SIZE]);
                     arr.add(new Vector<String>());
                     arr.add(new LinkedList<String>());
                } catch (Throwable e) {
                    break;
//                    System.out.println("OOME when adding or creating new object");
                }


            }
        }


    }

}
