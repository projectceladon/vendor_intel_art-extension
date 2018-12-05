class Main {
    final static int iterations = 10;

    public static byte simple_method() {
        byte    l3 = 127;
        return l3;
    }

    public static void main(String[] args) {
        byte workJ = -128;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = (byte)(simple_method() + workJ);
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
