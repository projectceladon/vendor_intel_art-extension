// InvokeDirect`:

class Test
{
    private int b(int div) {
       if (div == 10) {
            div = 0;
        }
        return 1/div;
    }
    public void a(int iter) {
        int c = 0;
        for (int i = 1; i < iter; i++) {
            c = b(i);
            System.out.println(c);
        }
        System.out.println(c);
   }

}


