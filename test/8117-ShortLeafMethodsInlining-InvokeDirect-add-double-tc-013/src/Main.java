class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 10.0;
        double workK = 15.0;

        System.out.println("Initial workJ value is " + workJ);

        for(double i = 0.0; i < iterations; i++) {
            try {
                workJ = test.shim(workJ, workK) + i;
            } catch (Exception e) {
                workJ = test.sh1m(workJ, workK) + i;
            }
        }
        System.out.println("Final workJ value is " + workJ);
    }
}
