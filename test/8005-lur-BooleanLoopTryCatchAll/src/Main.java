
public class Main {

    public class TestClass {
        private boolean value;
 
        public void setValue(boolean value) {
            this.value |= value;
        }

        public boolean getValue() {
            return value;
        }
    }

    final boolean testLoop() {
        TestClass tc = new TestClass();
        boolean a0 = true;
        boolean b0 = false;

        for (int i = 0; i < 10; i++) {
            tc.setValue(a0 || b0 || (i != 0));
        }
        return tc.getValue();
    }

    final boolean $noinline$testTryCatch() {
        boolean res = false;
        res |= testLoop();
        try {
            if (testLoop() == true) {
                throw new Exception("My exception");
            } 
        } catch (Exception e) {
            res ^= testLoop();
        } finally {
            res &= testLoop();
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
