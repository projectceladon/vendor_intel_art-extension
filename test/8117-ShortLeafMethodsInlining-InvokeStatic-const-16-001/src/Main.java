class Main {
    final static int iterations = 10;

    public static int simple_method() {
        int ll;
        ll = 55;
        return ll;
    }

    public static void main(String[] args) {
        int workJ = 0;

        System.out.println("Initial workJ value is " + workJ);

        for(int i = 0; i < iterations; i++) {
            workJ = simple_method() + i;
        }

        System.out.println("Final workJ value is " + workJ);
    }
}
