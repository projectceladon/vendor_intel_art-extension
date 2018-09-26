
public class Main
{
    public class A 
    {
        public char value;
    }

    public char testLoop()
    {
        A x;
        x = new A();
        char a0 = 'a';

        for (int i = 0; i < 60*1000*1000; i++)
        {
            a0 ++;
            x.value = a0;
        }

        return x.value;
    }

    public void test()
    {
        System.out.println(String.format ("\\u%04x", (int)testLoop()));
//        System.out.println(testLoop());
    }
    
    public static void main(String[] args)
    {
		new Main().test();        
    }    
}
