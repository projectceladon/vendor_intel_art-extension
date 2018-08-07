// InvokeVirtual

class Main
{
    final static int iterations = 11;

    public static int d(int div) {
        return 1 / div;
    }

    public static void main(String[] args)
    {
        Test test = new Test();
        float c = 0.0f;
        for (int i = 1; i < iterations; i++) {
            try {
                c = (float)d(0);
            } catch (Exception e) {
                c = test.b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
    }
}


