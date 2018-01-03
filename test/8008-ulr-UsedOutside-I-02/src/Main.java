
public class Main {

    // Test a loop with a variable used outside

    public int loop() {
        int used1 = 1;
        int used2 = 2;
        int used3 = 3;
        int used4 = 4;
        int used5 = 5;
        int invar1 = 15;
        int invar2 = 25;
        int invar3 = 35;
        int invar4 = 45;
        int invar5 = 55;


        for (int i = 0; i < 10000; i++) {
            used1 += invar1 + invar2;
            used2 -= used1 + invar2;
            used3 /= invar3 * used2 - 877778;
            used4 *= invar1 + used2++;
            used5 += used4 - invar5 * 333;

        }
        return used1 + used2 + used3 + used4 + used5;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
