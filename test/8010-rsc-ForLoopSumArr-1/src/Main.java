
public class Main {
    public static int testLoop() {
        int sum = 0;
        int[] iterations = new int[100];
        iterations[0] = 11;
        
        for (int i = 1; i < iterations.length; i++) {
            iterations[i] = iterations[i-1] * i;
            sum = sum + iterations[i];
        }
        return sum;
    }

    public void test() {
        System.out.println(testLoop());
    }

    public static void main(String[] args) {
        new Main().test();
    }
}


