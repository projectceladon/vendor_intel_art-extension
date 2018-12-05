
public class Main {

    // Test a loop with expected inlining and invariant hoisting


    public static int getValue() {
        return 5;
    }

    public static int loop() {
        int res = 0;
        int invar1 = 555;
        for (int i = 0; i < 10000; i++) {
            res += getValue() * 2 + invar1;
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
