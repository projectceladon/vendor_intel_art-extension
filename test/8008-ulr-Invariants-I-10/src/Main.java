
public class Main {

    // Negative because ConstantCalculationSinking can't have loads (iget)
    // Test a loop with invariants, which should be hoisted
    // Accumulation is also here. 

    int invar0 = 05;
    int invar1 = 15;
    int invar2 = 25;
    int invar3 = 35;
    int invar4 = 45;
    int invar5 = 55;
    int invar6 = 65;
    int invar7 = 75;

    public int loop() {
        int used0 = 0;
        int used1 = 1;
        int used2 = 2;
        int used3 = 3;
        int used4 = 4;
        int used5 = 5;
        int used6 = 6;
       for (int i = 0; i < 10000; i++) {
            used0 += invar0 + invar1;
            used1 += invar1 + invar2 + invar0;
            used2 += invar2 + invar3;
            used3 += invar3 + invar4;
            used4 += invar4 + invar5 + invar4;
            used5 += invar5 + invar6;
            used6 += invar6 + invar7;
        }
        return used0 + used1 + used2 + used3 + used4 + used5 + used6;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
