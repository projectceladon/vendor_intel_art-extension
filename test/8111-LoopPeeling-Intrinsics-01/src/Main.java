

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

    public static double intrinsic_inlined(int x) {
        return Math.sqrt(x);
    }


    /* Peeling candidate: 1 intrinsic call */
    public double test_01(int x) {
        double sum = 0;
        sum += x;
        for (int i = 0; i < iterations; i++) {
            sum = Math.sqrt(x);
        }
        
        sum += 0.5d;

        return sum;
    }

    /* Peeling candidate: 3 intrinsic calls */
    public double test_02(int x) {
        double sum = 0;
        sum += x;
        for (int i = 0; i < iterations; i++) {
            sum = Math.sqrt(x) + Math.sqrt(Math.sqrt(x));
        }
        
        sum += 0.5d;

        return sum;
    }

    /* Peeling candidate: 3 intrinsic calls */
    public double test_03(int x) {
        double sum1 = x;
        double sum2 = x + 1;
        double sum3 = x + 2;
        for (int i = 0; i < iterations; i++) {
            sum1 = Math.sqrt(x);
            sum2 = Math.abs(x);
            sum3 = Math.sqrt(x+2);
        }
        
        sum2 += 0.5d;

        return sum1 + sum2 + sum3;
    }

    /* Peeling candidate: 1 intrinsic calls, inlined */
    public double test_04(int x) {
        double sum = x;
        Cls1_ext c = new Cls1_ext();
        for (int i = 0; i < iterations; i++) {
            sum = c.inlined_intrinsic(x);
        }
        
        sum += 0.5d;

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


