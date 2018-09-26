
public class Main {
    public static int testLoop() {
        int m = 10000;
        int res = 0;
        int i = 0;
        do {
            res = i*(i-1)/3;
            i++;
        } while (i < m);
        return res;
    }

    public void test() {
        System.out.println(testLoop());
    }

    public static void main(String[] args) {
        new Main().test();
    }
}


