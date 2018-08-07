
public class Main {
    public char[]  value = new char[3];

    public final char testLoop(char a0) {
        int iterations = 8;
        for (int i = 0; i < iterations; i++) {
            a0 = 'b';
            value[0] = a0;
            value[1] = (char)(i * 0x11);
        }
        return value[1];
    }

    final int $noinline$testTryCatch(char a) {
        int res = 0;
        res += testLoop(a);
        try {
            if (testLoop(a) > 0) {
                throw new Exception("My exception");
            } 
        } catch (Exception e) {
            res ^= testLoop(a);
        } finally {
            res *= testLoop(a);
        }
        return res;
    }

    public void test() {
        System.out.println($noinline$testTryCatch('a'));
    }
    
    public static void main(String[] args) {
        new Main().test();        
    }    
}
