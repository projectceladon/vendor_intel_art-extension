class Main {
    final static int iterations = 10;

    public static int simple_method() {
        int     l5;
        l5 = 2147483647;
        return l5;
    }

    public static void main(String[] args) {
        int workJ = -2147483648;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + workJ;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
