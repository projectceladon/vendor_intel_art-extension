
public class Main {
    public static int testLoop() {
        int m = 50000;
        int sum = 0;
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


