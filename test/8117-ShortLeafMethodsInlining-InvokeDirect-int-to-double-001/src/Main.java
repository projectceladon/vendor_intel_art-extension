class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 123456789;
        double workK = 0.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(double i = 0.0d; i < iterations; i++) {
            workK = test.shim(workJ) + i;
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
