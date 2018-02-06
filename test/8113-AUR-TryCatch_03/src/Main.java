

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;
import java.io.*;

public class Main {

    public int field = 5;
    
    private int getSum(int arg) {
        return arg + 20;
    }



    // simple AUR example for try-catch block with DivZeroCheck in try section and in catch section and in finally section: 
    // environment of DivZeroCheck and unused instructions are now removed inside TryBoundary entry-exit, also removed outside;
    public int runTest (int n, Main other) {
        int sum1 = 0;
        int sum2 = 0;
        int sum3 = 0;
        for (int i = 0; i < 15000; i++) {
        try {
           // n - 10 == 0 : EXCEPTION!
           sum1 = 100/(n + i - 10);
        } catch (ArithmeticException ae) {
            sum2 = n*100/(n - i - 11);
            other.field += 1;
        } finally {
            sum3 = n*100/(n + i - 9);
            other.field += 10;
        }
        }
        return other.field;

    }

    // wrapper method for runTest
    public int test(int n) {
        Main other = new Main();
        return runTest(n, other);
    }

    // wrapper method for runTest with GC stress threads running
    public String testWithGCStress(int n) {
        String res = "";
        Thread t = new Thread(new Runnable(){
                public void run() {
                new StressGC().stressGC();
                }
                });

        t.start();
        try {
            res += test(n);
        } catch (Throwable e) {
            res += e;
        }
        try {
            t.join();
        } catch (InterruptedException ie) {
        }
        return res;
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
                    StringWriter sw = new StringWriter();
                    StackTraceElement[] ste = e.getCause().getStackTrace();
                    System.out.println(e.getCause());
                    for (int i = 0; i < ((ste.length < 2)?ste.length:2); i++) {
                        System.out.println(ste[i]);
                    }

                }
            }
        }



    }

    public static void main(String[] args)
    {
        new Main().runTests();
    }

}


