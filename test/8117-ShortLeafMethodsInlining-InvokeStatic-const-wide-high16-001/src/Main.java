class Main {
    final static int iterations = 10;

    public static double simple_method() {
        double  l8;
        l8 = 100.0d;
        return l8;
    }

    public static void main(String[] args) {
        double workJ = Double.MIN_VALUE;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
