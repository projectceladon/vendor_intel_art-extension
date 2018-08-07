
public class Main {
    public static double testLoop() {
        int m = 80000;
        double res = 1;
        for (int i = 1; i < m; i++) {
            res = res + i * (i+1);
        }
        return res;
    }

    public void test() {
        System.out.println(testLoop());
    }

    public static void main(String[] args) {
        new Main().test();
    }
}


