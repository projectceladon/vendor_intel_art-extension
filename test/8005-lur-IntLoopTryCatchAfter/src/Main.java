
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

    final int testLoop(TestClass tClass) {
        for (int i = 0; i < 10; i++) {
            tClass.setValue(i);
        }
        return tClass.getValue();
    }

    final int testTryCatch() {
        int res = 0;
        TestClass tClass = new TestClass();
        try {
            throw new Exception("My exception"); 
        } catch (Exception e) {
        } finally {
        }
        res *= testLoop(tClass);
        return res;
    }

    public void test() {
        System.out.println(testTryCatch());
    }

    public static void main(String[] args) {
        new Main().test();
    }    
}
