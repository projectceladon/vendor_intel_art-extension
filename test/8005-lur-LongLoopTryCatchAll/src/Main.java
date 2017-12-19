
public class Main {

    public class TestClass {
        private long value;
 
        public void setValue(long value) {
            this.value += value;
        }

        public long getValue() {
            return value;
        }
    }

    final long testLoop() {
        TestClass tc = new TestClass();
        long a = 0x7000000000000007L;

        for (int i = 0; i < 10; i++) {
            a++;
            tc.setValue(a);
        }
        return tc.getValue();
    }

    final long $noinline$testTryCatch() {
        long res = 0;
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
        System.out.println($noinline$testTryCatch());
    }

    public static void main(String[] args) {
        new Main().test();
    }    
}
