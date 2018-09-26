
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
        try { 
        for (int i = 0; i < 60; i++)
        {
            a0 ++;
            x.value = a0;
        }

        throw new Exception();}
        catch (Exception e){ x.value++; }
        finally { x.value-=1; }


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
