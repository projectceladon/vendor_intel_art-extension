
public class Main {
    public static long testLoop() {
        int m = 120000;
        long sum = 0;
        for (int i = 0; i < m; i++) {
            sum +=  i;
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


