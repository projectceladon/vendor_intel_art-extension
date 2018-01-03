
public class Main {

    // M: was: negative because local_var = field; is not hoisted; lately: positive because LHSS is now before unused loop removal
    // N: positive because local_var = field; is hoisted
    // Test a loop with invariants, which should be hoisted 

    final int invar1 = 15;
    final int invar2 = 25;
    final int invar3 = 35;
    final int invar4 = 45;

    public int loop() {
        int used1 = 1;
        int used2 = 2;
        int used3 = 3;
        int used4 = 4;
        int i = 0;
        while (i < 10000)  {
            used1 = invar1 + invar2;
            used2 = invar2 - invar3;
            used3 = invar3 * invar4;
            used4 = invar1 * invar2 - invar3 + invar4;
            i++;
        }
        return used1 + used2 + used3 + used4;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
