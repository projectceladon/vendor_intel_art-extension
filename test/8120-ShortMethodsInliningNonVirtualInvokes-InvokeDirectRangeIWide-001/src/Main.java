class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        double nextThingy = -100000.00;
        double d1 = 1.0;
        double d2 = 2.0;
        double d3 = 3.0;
        double d4 = 4.0;
        double d5 = 5.0;
        double d6 = 6.0;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.gimme(d1, d2, d3, d4, d5, d6) + i*i;
            test.hereyouare(nextThingy, d1, d2, d3, d4, d5, d6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
