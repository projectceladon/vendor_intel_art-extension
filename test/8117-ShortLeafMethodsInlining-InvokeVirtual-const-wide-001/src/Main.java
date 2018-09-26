class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        long workJ = -9223372036854775808l;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = test.simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
