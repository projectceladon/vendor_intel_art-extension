class Main {
    final static int iterations = 10;

    public static double simple_method() {
        double  l8;
        l8 = Double.MIN_VALUE;
        return l8;
    }

    public static void main(String[] args) {
        double workJ = 10.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
