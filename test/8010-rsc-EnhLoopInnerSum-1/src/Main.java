
public class Main {
    public static int testLoop() {
        int sum = 0;
        int[] arr1 = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90};
        int[] arr2 = {90, 80, 70, 60, 50, 40, 30, 20, 10, 0};

        for (int i : arr1) {
            for (int j : arr2) {
                sum +=  i + j;
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


