

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;
import java.io.*;

public class Main {


    class A  {
        protected int fieldA = 1;
    
    }
    class B extends A {}
    class C extends A {}
    class D extends B {}
    class E extends D {
        class F {
            int fieldE = 100;
        }
    }
    static class G {
        static int fieldG = 1000;
    }

    public int field = 5;
    
    private int getSum(int arg) {
        return arg + 20;
    }



    // simple AUR example for NewInstance
    public int runTest (int n, Main other, A a) {
        int sum = 0;

        sum += (new A()).fieldA;
        sum += new Main().field;
        sum += (new B()).fieldA;
        sum += (new C()).fieldA;
        sum += (new D()).fieldA;
        sum += (new E()).fieldA;
        sum += ((new E()).new F()).fieldE;
        sum += Main.G.fieldG;
        sum += new Main().field;

        return sum;

    }

    // wrapper method for runTest
    public int test(int n) {
        A a = new A();
        B b = new B();
        C c = new C();
        D d = new D();

        Main other = new Main();
        return runTest(n, other, a) + runTest(n, other, b) + runTest(n, other, c) + runTest(n, other, d);
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


