
public class Main {

    // M: positive because loop is bottom tested
    // Test a loop with invariants, which should be hoisted 

    int invar1 = 15;
    int invar2 = 25;
    int invar3 = 35;
    int invar4 = 45;

    public int loop() {
        int used1 = 1;
        int used2 = 2;
        int used3 = 3;
        int used4 = 4;
        for (int i = 0; ; i +=2 ) {
            used1 = invar1 + invar2;
            used2 = used1 + invar2 - invar3;
            used3 = used2 + invar3 * invar4;
            used4 = used3 + invar1 * invar2 - invar3 + invar4;
            if (i>=10000) break;
        }
        return used1 + used2 + used3 + used4;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
