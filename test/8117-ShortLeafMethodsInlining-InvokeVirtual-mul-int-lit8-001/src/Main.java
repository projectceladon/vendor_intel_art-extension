class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 2;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = test.simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
