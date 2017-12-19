
public class Main {

    // M: negative because type casts are not sunk/hoisted 
    // Test a loop with invariants, which should be hoisted

    public byte loop() {
        byte used1 = 1;
        byte used2 = 2;
        byte used3 = 3;
        byte used4 = 4;
        byte invar1 = 15;
        byte invar2 = 25;
        byte invar3 = 35;
        byte invar4 = 45;


        for (byte i = 0; i < 127; i++) {
            used1 -= (byte) (invar1 + invar2);
            used2 *= (byte) (invar2 - invar3);
            used3 += (byte) (invar3 * invar4);
            used4 /= (byte) (invar1 * invar2 - invar3 + invar4);
        }
        return (byte) (used1 + used2 + used3 + used4);
    }

    public static void main(String[] args) {
        byte res = new Main().loop();
        System.out.println(res);
    }
}
