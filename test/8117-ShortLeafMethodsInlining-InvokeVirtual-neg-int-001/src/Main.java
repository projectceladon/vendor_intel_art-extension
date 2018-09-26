class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 123456789;
        int workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = test.simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
