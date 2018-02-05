class Main {
    final static int iterations = 10;
    static int j = 0;

    public static int simple_method(int jj) {
        jj = 32767 - jj;
        return jj;
    }

    public static void main(String[] args) {
        int nextJ = -10;

        System.out.println("Initial nextJ value is " + nextJ);

        for(int i = 0; i < iterations; i++) {
            nextJ = simple_method(i) + i;
        }

        System.out.println("Final nextJ value is " + nextJ);
    }
}
