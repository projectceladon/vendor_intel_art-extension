// InvokeVirtual

class Main
{
    final static int iterations = 11;
    public static void main(String[] args)
    {
        Test test = new Test();
        double c = 0.0d;
        for (int i = 1; i < iterations; i++) {
            c = test.b(i);
            System.out.println(c);
        }
        System.out.println(c);
    }
}


