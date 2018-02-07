class Main {
    final static int iterations = 10;

    public static byte simple_method(int jj) {
        byte ii;
        ii = (byte)jj;
        return ii;
    }

    public static void main(String[] args) {
        int workJ = 123456789;
        byte workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = (byte)(simple_method(workJ) + i);
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
