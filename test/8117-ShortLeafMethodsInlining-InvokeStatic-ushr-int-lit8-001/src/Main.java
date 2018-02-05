class Main {
    final static int iterations = 10;

    public static int simple_method(int jj) {
        jj = jj >>> 0xF;
        return jj;
    }

    public static void main(String[] args) {
        int workJ = 0xF8E9;

        System.out.format("Initial workJ value is %o%n", workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method(workJ) + i;
        }

        System.out.format("Final workJ value is %o%n", workJ);
    }
}
