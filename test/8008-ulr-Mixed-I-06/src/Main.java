
public class Main {

    // Test a loop with expected inlining and invariant hoisting
    // Several loops in a row, one of them shouldn't be removed


    public static int getValue() {
        return 5;
    }


    public static int foo(int x) {
        return x;
    }

    public static int loop() {
        int res = 0;
        int used1 = 1;
        int invar1 = 555;
        for (int i = 0; i < 10000; i++) {
            res += getValue() * 2 + invar1;
            used1 += 5;
        }
        res += 5;
        //shouldn't be removed
        for (int i = 0; i < 10000; i++) {
            res += getValue() * 5 - invar1 + i;
            used1 = 8;
        }
 
        res += 3;
        for (int i = 0; i < 10000; i++) {
            res += getValue() * 3 + invar1;
            used1 -= 2;
        }
       
        int j = -1;
        used1 = 123;
        return res + used1;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
