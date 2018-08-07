
public class Main {
    // Test a loop that is not simple

    public int loop() {
        int invar1 = 555;
        int[] arr = new int[10000];
        int res = 0;
        for (int i = 0; i < 10000; i++) {
            res += i + invar1;
            arr[i] -= invar1 + i;
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
