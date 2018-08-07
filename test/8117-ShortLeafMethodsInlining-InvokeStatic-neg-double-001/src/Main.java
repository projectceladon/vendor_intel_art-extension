class Main {
    final static int iterations = 10;

    public static double simple_method(double jj) {
        return (-jj);
    }

    public static void main(String[] args) {
        double workJ = 1.7976931348623157e308;
        double workK = 0.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
