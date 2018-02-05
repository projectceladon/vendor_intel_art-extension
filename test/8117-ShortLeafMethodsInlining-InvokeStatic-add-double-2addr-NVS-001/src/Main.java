class Main {
    final static int iterations = 10;

    public static double simple_method(double jj, double ii) {
        jj = ii + jj;
        jj = ii + jj;
        return jj;
    }

    public static void main(String[] args) {
        double workJ = 10.0;
        double workK = 15.0;

        System.out.println("Initial workJ value is " + workJ);

        for(double i = 0.0; i < iterations; i++) {
            workJ = simple_method(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
