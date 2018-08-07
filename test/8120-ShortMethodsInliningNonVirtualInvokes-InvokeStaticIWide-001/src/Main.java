class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        double nextThingy = -10.0;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = Test.getThingies(test) + i*i;
            Test.setThingies(test, nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
