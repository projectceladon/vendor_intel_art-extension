
public class Main {

    // M: negative because type casts are not sunk/hoisted
    // Test a loop with invariants, which should be hoisted 

    public long loop() {
        long used1 = 1;
        long used2 = 2;
        long used3 = 3;
        long used4 = 4;
        long invar1 = 1592834792834728937l;
        long invar2 = 2520728572987878988l;
        long invar3 = 3528394283472384834l;
        long invar4 = 4528798279287589927l;


        for (long i = 0; i < 10000; i++) {
            used1 += (long) (invar1 + invar2);
            used2 -= (long) (invar2 - invar3);
            used3 /= (long) (invar3 * invar4);
            used4 *= (long) (invar1 * invar2 - invar3 + invar4);
        }
        return (long) (used1 + used2 + used3 + used4);
    }

    public static void main(String[] args) {
        long res = new Main().loop();
        System.out.println(res);
    }
}
