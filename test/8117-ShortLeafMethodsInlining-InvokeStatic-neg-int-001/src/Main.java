class Main {
    final static int iterations = 10;

    public static int simple_method(int jj) {
        return (-jj);
    }

    public static void main(String[] args) {
        int workJ = 123456789;
        int workK = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workK = simple_method(workJ) + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
