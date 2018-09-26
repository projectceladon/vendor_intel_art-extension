
public class Main {

    // M: negative because type casts are not sunk/hoisted
    // Test a loop with invariants, which should be hoisted 

    public int loop() {
        float used1 = 1;
        float used2 = 2;
        float used3 = 3;
        float used4 = 4;
        float invar1 = 15;
        float invar2 = 25;
        float invar3 = 35;
        float invar4 = 45;


        for (float i = 0; i < 10000; i++) {
            used1 += invar1 + invar2;
            used2 *= invar2 - invar3;
            used3 /= invar3 * invar4;
            used4 -= invar1 * invar2 - invar3 + invar4;
        }
        return Float.floatToIntBits(used1 + used2 + used3 + used4);
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
