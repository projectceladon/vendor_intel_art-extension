
public class Main {

    // M: negative because type casts are not sunk/hoisted
    // Test a loop with invariants, which should be hoisted 

    public short loop() {
        short used1 = 1;
        short used2 = 2;
        short used3 = 3;
        short used4 = 4;
        short invar1 = 15;
        short invar2 = 25;
        short invar3 = 35;
        short invar4 = 45;


        for (short i = 0; i < 10000; i++) {
            used1 += (short) (invar1 + invar2);
            used2 -= (short) (invar2 - invar3);
            used3 *= (short) (invar3 * invar4);
            used4 %= (short) (invar1 * invar2 - invar3 + invar4);
        }
        return invar1;
    }

    public static void main(String[] args) {
        short res = new Main().loop();
        System.out.println(res);
    }
}
