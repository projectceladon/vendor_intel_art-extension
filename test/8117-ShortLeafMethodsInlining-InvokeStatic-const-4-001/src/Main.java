class Main {
    final static int iterations = 10;

    public static boolean simple_method() {
        boolean l1;
        l1 = false;
        return l1;
    }

    public static void main(String[] args) {
        boolean workJ = true;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() | workJ;
        }
        System.out.println("Final workJ value is " + workJ);
    }
}
