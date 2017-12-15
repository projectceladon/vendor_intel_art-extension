
public class Main {
    public static int testLoop() {
        int sum = 0;
        int[] iterations = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};

        for (int i : iterations) {
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


