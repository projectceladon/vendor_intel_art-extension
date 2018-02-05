class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        double workJ = 4.9e-324;
        int workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.simple_method(workJ) + i;
            workJ = (double)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
