
public class Main {

    // Test a loop that does not have a known number of iterations


    public int loop(int n) {
        int res = 0;
        for (int i = 0; i < n*1000; i++) {
            ;
        }
        return res;
    }

    public static void main(String[] args) {
        int res = new Main().loop(10);
        System.out.println(res);
    }
}
