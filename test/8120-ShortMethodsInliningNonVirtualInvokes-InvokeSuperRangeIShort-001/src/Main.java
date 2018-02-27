class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        short nextThingy = -32768;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        System.out.println("Initial nextThingy is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short) (test.getThingies(t1, t2, t3, t4, t5, t6) + 1);
            test.setThingies(nextThingy, t1, t2, t3, t4, t5, t6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
        System.out.println("Final test.thingies value is " + test.thingies);

    }
}
