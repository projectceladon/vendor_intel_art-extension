
public class Main {

    // Test a loop that does not have a known number of iterations

    int bound = 10000;

    public int loop() {
        int res = 0;
        for (int i = 0; i < bound; i++) {
            ;
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop();
        System.out.println(res);
    }
}
