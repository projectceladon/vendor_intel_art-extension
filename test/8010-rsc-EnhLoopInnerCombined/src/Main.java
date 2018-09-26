
public class Main {
    public static double testLoop() {
        int m = 10000;
        double sum = 1;
        int[] arr1 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        int[] arr2 = {100, 90, 80, 70, 60, 50, 40, 30, 20, 10};

        for (int k = 1; k <= m; k++) {
            for (int i : arr1) {
                sum = sum + k * i;
                for (int j : arr2) {
                    sum = sum + k * j;
                }
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


