
// negative

public class Main
{
    class A {
        int fieldA;
        int dummy;
    }

    class B {
        int dummy;
        int fieldB;
    }

    public void testLoop()
    {
        A inst1 = new A();
        B inst2 = new B();
        int iterations = 50;
        for (int i = 0; i < iterations*1000*1000; i++)
        {
            int a = inst1.fieldA;
            inst1.fieldA = a + i;
            int b = inst2.fieldB;
            inst2.fieldB = b + 2 * i;
        }
        System.out.println(inst1.fieldA);
        System.out.println(inst2.fieldB);
    }

    public void test()
    {
        testLoop();
    }
    
    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
