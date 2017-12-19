
public class Main {

    // M: negative (loop is not countable)
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
        
        float i = 0.5f;

        do {
            used1 = invar1 + invar2;
            used2 = invar2 - invar3;
            used3 = invar3 * invar4;
            used4 = invar1 * invar2 - invar3 + invar4;
            i += 0.5f;
        } while (i < 5000.25f);

        return Float.floatToIntBits(used1 + used2 + used3 + used4);
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
