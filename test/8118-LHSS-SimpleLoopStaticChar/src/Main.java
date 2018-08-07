
public class Main
{
	public static class A 
    {
        public static char value;
    }

    public char testLoop(char a0)
    {
        A x;
        x = new A();
// pass a0, via parameter to isolate LHSS pass from other optimization passes

        for (int i = 0; i < 60; i++)
        {
            a0 ++;
            x.value = a0;
        }

        return x.value;
    }

    public void test()
    {
        System.out.println(String.format ("\\u%04x", (int)testLoop('a')));
//        System.out.println(testLoop('a'));
    }
    
    public static void main(String[] args)
    {
		new Main().test();        
    }    
}
