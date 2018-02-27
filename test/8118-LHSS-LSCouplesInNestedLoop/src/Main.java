
// positive

public class Main
{
    class A {
        int fieldA;
        int fieldB;
    }
//    public static A field;

    public void testLoop()
    {
        A inst = new A();
        int iterationsnested = 50;
        int iterations = 10;
        int c = 0;
        for (int j = 0; j < iterations; j++)
        {
            for (int i = 0; i < iterationsnested; i++)
            {
                int a = inst.fieldA;
                inst.fieldA = a + i;
                int b = inst.fieldB;
                inst.fieldB = b + 2 * i;
            }
           
            System.out.println(inst.fieldA);
            System.out.println(inst.fieldB);
            c = inst.fieldA + inst.fieldB;
        }
        System.out.println(c);
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
