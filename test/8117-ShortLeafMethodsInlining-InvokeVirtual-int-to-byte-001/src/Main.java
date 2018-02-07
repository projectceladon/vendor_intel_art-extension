class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int workJ = 123456789;
        byte workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = (byte)(test.simple_method(workJ) + i);
            workJ = (int)workK;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
