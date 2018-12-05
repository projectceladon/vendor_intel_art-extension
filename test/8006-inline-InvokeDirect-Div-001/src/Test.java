// InvokeDirect`:

class Test
{
    private float b(int div) {
        return 1.0f/div;
    }
    public void a(int iter) {
        float c = 0.0f;
        for (int i = 1; i < iter; i++) {
            c = b(i);
            System.out.println(c);
        }
        System.out.println(c);
   }

}


