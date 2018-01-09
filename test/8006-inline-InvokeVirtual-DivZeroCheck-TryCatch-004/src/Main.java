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
        long c = 0L;
        for (int i = 1; i < iterations; i++) {
            try {
                c = (long)d(0);
            } catch (Exception e) {
                // ignore
            } finally {
                c = test.b(i);
            }
            System.out.println(c);
        }
        System.out.println(c);
    }
}
