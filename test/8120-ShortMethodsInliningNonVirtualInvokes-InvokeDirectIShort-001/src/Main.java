class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
//        short nextThingy = -32768;
        short nextThingy = -11111;

        System.out.println("Initial nextThingy value is " + nextThingy);
        System.out.println("Initial test.thingies value is " + test.thingies);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short)(test.gimme() + 1);
            test.hereyouare(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
        System.out.println("Final test.thingies value is " + test.thingies);

    } 
}
