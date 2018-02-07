class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 10.0;
        double workK = 15.0;

        System.out.println("Initial workJ value is " + workJ);

        for(double i = 0.0; i < iterations; i++) {
            workJ = test.shim(workJ, workK) + i;
        }

        double j = -1.0;
        try {
            throw new Exception("Test");
        } catch (Exception e) {
            j += 1.0;
        }
        workJ += j;

        System.out.println("Final workJ value is " + workJ);
    }
}
