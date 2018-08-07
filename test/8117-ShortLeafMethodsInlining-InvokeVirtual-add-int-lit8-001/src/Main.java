class Main {
    final static int iterations = 10;

    public static void main(String[] args) {
        Test test = new Test();
        int nextJ = -10;

        System.out.println("Initial nextJ value is " + nextJ);

        for(int i = 0; i < iterations; i++) {
            nextJ = test.simple_method(i) + i;
        }

        System.out.println("Final nextJ value is " + nextJ);
    }
}
