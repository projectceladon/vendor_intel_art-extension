class Main {
    final static int iterations = 10;

    public static long simple_method(long jj) {
        return (-jj);
    }

    public static void main(String[] args) {
        long workJ = 0x123456789l;
        long workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
