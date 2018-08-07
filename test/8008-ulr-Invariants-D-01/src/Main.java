
public class Main {

    // M: negative because type casts are not sunk/hoisted
    // Test a loop with invariants, which should be hoisted 

    public long loop() {
        double used1 = 1;
        double used2 = 2;
        double used3 = 3;
        double used4 = 4;
        double invar1 = 234234234234l;
        double invar2 = 2523423423423424l;
        double invar3 = 35234234234234234l;
        double invar4 = 45234234234234234l;


        for (double i = 0; i < 10000; i++) {
            used1 += invar1 + invar2;
            used2 *= invar2 - invar3;
            used3 -= invar3 * invar4;
            used4 /= invar1 * invar2 - invar3 + invar4;
        }
        return Double.doubleToLongBits(used1 + used2 + used3 + used4);
    }

    public static void main(String[] args) {
        long res = new Main().loop();
        System.out.println(res);
    }
}
