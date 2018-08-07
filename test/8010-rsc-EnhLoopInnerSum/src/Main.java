
public class Main {
    public static int testLoop() {
        int m = 10000;
        int sum = 0;
        int[] iterations = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};

        for (int j = 0; j < m; j++) {
            for (int i : iterations) {
                sum +=  i;
            }
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


