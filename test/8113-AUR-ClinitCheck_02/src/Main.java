

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;
import java.io.*;

public class Main {

    public int field = 5;
    
    private int getSum(int arg) {
        return arg + 20;
    }


    static class C {
        int count;
        static int stCount;

        public static int value  = 50;

        {
            count = 0;
        }

        static {
            stCount = -1;
            if (value == 50) {throw new ArithmeticException();}

        }

        public C(int n) {
            count = n;
        }

    }
    // ClinitCheck - ExceptionInInitializerError
    public int runTest(C c, int x) {

        int sum = C.stCount % 5;
        return sum;
    }

    // wrapper method for runTest
    public int test(int x) {
        C c = new C(x);
        Main other = new Main();
        //return runTest(x, other, c);
        return runTest(c, x);
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
            res += e.getClass();
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
                    System.out.println(e.getCause().getClass());
                    for (int i = 0; i < ((ste.length < 1)?ste.length:1); i++) {
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


