

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;
import java.io.*;

public class Main {

    public int field = 5;
    
    private int getSum(int arg) {
        return arg + 20;
    }



    // will not be inlined
    public static int $noinline$foo(int x) {
        return (x+1)*(x-1);
    }


    // AUR example for InvokeStaticOrDirect:  invoke with side effects shouldn't be removed even if they have no more users
    public double runTest (int x, int n, Main m, Main other, int a, int b, StringBuffer s1, int c, int d) {


      // if it is deleted, Dead Code Elimination will remove invoke
      n = n + x;

      int result = 0;
      for (int i = 0; i < 15000; i++) {
          result += i/2;
      }
      // side effect: memory read
      c = s1.lastIndexOf("s1s1", c);
      return m.field + result;
    }

    // wrapper method for runTest
    public int test(int n) {
        Main m = new Main();
        Main other = new Main();
        Random rand = new Random();
        Calendar rightNow = Calendar.getInstance();
        return (int)runTest(11, n, m, other, 45, 7, new StringBuffer("s1s1s1"), 2, 1);
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
                    // To workaround difference between android detailed message and java brief one:
                    System.out.println(e.getCause().getClass());
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


