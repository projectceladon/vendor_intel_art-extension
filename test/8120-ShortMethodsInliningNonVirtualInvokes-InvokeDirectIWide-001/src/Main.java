class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        double nextThingy = -100000.00;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = test.gimme() + i*i;
            test.hereyouare(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
