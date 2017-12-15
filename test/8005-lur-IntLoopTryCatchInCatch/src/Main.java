
public class Main {

    public class TestClass {
        private int value;
 
        public void setValue(int value) {
            this.value += value;
        }

        public int getValue() {
            return value;
        }
    }

    final int testLoop() {
        TestClass tc = new TestClass();
        int a = 0x70000007;

        for (int i = 0; i < 10; i++) {
            a++;
            tc.setValue(a);
        }
        return tc.getValue();
    }

    final int $noinline$testTryCatch() {
        int res = 0;
        try {
            throw new Exception("My exception"); 
        } catch (Exception e) {
            res ^= testLoop();
        } finally {
        }
        return res;
    }

    public void test() {
        System.out.println($noinline$testTryCatch());
    }

    public static void main(String[] args) {
        new Main().test();
    }    
}
