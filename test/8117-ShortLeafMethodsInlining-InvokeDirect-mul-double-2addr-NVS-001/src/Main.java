class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 2.0;
        double workK = 0.5;

        System.out.println("Initial workJ value is " + workJ);

        for(double i = 0.0; i < iterations; i++) {
            workJ = test.shim(workJ, workK) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
