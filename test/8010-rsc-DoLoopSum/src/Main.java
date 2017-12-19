
public class Main {
    public static int testLoop() {
        int m = 50000;
        int sum = 0;
        int i = 0;
        do {
            sum +=  i;
            i++;
        } while (i < m);
        return sum;
    }

    public void test() {
        System.out.println(testLoop());
    }

    public static void main(String[] args) {
        new Main().test();
    }
}


