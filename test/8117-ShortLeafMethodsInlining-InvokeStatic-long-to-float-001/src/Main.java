class Main {
    final static int iterations = 10;

    public static float simple_method(long jj) {
        float ii;
        ii = (float)jj;
        return ii;
    }

    public static void main(String[] args) {
        long workJ = 0xFFEFAF23;
        float workK = 0.0f;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
            workJ = (long)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
