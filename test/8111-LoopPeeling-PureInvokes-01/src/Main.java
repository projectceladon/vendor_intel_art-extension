

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class Main {
    public final int iterations = 10000;
    public static int s_field1 = 33;
    private double i_field1 = 0.00001234500012345d;
    protected double i_field2 = 1.00001234500012345d;
    public double i_field3 = 2.00001234500012345d;
    public final static int s_final_field1 = 100;
    private volatile int i_volatile_field1 = 0;
    Cls1_ext c_e = new Cls1_ext();


    public static int foo_inlined(int x) {
        return x;
    }

    public static int pure_inlined(String s, int n) {
        return s.lastIndexOf(n);
    }


    /* Peeling candidate: 1 pure call */
    public int test_01(int x) {
        String str = "abc";
        int sum = 0;
        for (int i = 0; i < iterations; i++) {
            sum = str.lastIndexOf(x);
        }
        

        return sum;
    }


    /* Peeling candidate: 2 pure calls */
    public int test_02(int x) {
        String str = "abc";
        String stra = "a";
        String strb = "b";
        int sum = 0;

        for (int i = 0; i < iterations; i++) {
            sum = str.lastIndexOf(stra) + str.lastIndexOf(strb);
        }
        

        return sum;
    }

    /* Peeling candidate: 3 pure calls */
    public int test_03(int x) {
        String str = "abc";
        String stra = "a";
        String strb = "b";
        String strc = "c";
        int sum = 0;

        for (int i = 0; i < iterations; i++) {
            sum = str.lastIndexOf(stra) + str.lastIndexOf(strb) + str.lastIndexOf(strc);
        }
        

        return sum;
    }


    /* Peeling candidate: 1 pure call, inlined */
    public int test_04(int x) {
        String str = "abc";
        int sum = 0;
        for (int i = 0; i < iterations; i++) {
            sum = pure_inlined(str, 2);
        }
        

        return sum;
    }

    public void runTests() {
        Class c = new Main().getClass();
        Method[] methods = c.getDeclaredMethods();
        Method tempMethod;
        for (int i = 1; i < methods.length; i++) {
            int j = i;
            while (j > 0 &&  methods[j-1].getName().compareTo(methods[j].getName()) > 0) {
                tempMethod = methods[j];
                methods[j] = methods[j-1];
                methods[j-1] = tempMethod;
                j = j - 1;
            }
        }

        Object[] arr = {null};
        for (Method m: methods){
            if (m.getName().startsWith("test")){
                try {
                    String names[] = c.getName().split("\\.");
                    String testName = names[names.length-1];
                    System.out.println("Test "+testName+"; Subtest "+m.getName()+"; Result: "+ m.invoke(this, 10));
                } catch (IllegalArgumentException e) {
                    e.printStackTrace();
                } catch (IllegalAccessException e) {
                    e.printStackTrace();
                } catch (InvocationTargetException e) {
                    e.printStackTrace();
                }
            }
        }



    }

    public static void main(String[] args)
    {
        new Main().runTests();
    }

}


