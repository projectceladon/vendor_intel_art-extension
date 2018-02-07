class Main {
    final static int iterations = 10;

    public static short simple_method() {
        short   l4;
        l4 = 32767;
        return l4;
    }

    public static void main(String[] args) {
        short workJ = -32768;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = (short)(simple_method() + workJ);
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
