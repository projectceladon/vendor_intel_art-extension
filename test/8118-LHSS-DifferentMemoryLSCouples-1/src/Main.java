
// positive

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
        for (int i = 0; i < iterations; i++)
        {
            int a = inst1.fieldA;
            inst1.fieldA = a + i;
            int d = inst2.dummy;
            inst2.dummy = d + i *3;
            int b = inst2.fieldB;
            inst2.fieldB = b + 2 * i;
        }
        System.out.println(inst1.fieldA);
        System.out.println(inst2.dummy);
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
