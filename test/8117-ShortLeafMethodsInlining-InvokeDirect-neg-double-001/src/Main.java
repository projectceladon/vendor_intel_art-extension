class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 1.7976931348623157e308;
        double workK = 0.0d;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.shim(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
