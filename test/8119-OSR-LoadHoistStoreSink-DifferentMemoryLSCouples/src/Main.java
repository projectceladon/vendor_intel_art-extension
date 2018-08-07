
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
        int iterations = 50;
        for (int i = 0; i < iterations*1000*1000; i++)
        {
            int a = inst.fieldA;
            inst.fieldA = a + i;
            int b = inst.fieldB;
            inst.fieldB = b + 2 * i;
        }
        System.out.println(inst.fieldA);
        System.out.println(inst.fieldB);
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
