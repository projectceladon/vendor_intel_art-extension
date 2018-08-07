
public class Main {
    public int[] value = new int [3];

    public final int testLoop() {
        int a0 = 0x7007;

        for (int j = 0; j < 10; j++) {
            a0++;
            value[0] = a0;
        }
        return value[0];
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        res += testLoop();
        try {
            if (testLoop() > 0) {
                throw new Exception("My exception");
            } 
        } catch (Exception e) {
            res ^= testLoop();
        } finally {
            res *= testLoop();
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
