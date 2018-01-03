
public class Main {

    // Test a loop with expected inlining

    public static int loop() {
        int res = 0;
        for (int i = 0; i < 10000; i++) {
            res += Math.max(i, i + 1) + Math.min(i, i + 1);
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
