
public class Main
{
    public class A 
    {
        public float value;
    }

    public float testLoop()
    {
        A x;
        x = new A();
        float a0 = 1.7777777f;

        for (float i = 0.2f; i < 10*1000*1000; i += 0.9f)
        {
            x.value = a0;
            a0 += i;
        }

        return x.value;
    }

    public void test()
    {
        System.out.println(testLoop());
    }

    public static void main(String[] args)
    {
        new Main().test();        
    }    
}
