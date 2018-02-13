class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        short nextThingy = 0;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short)(Test.getThingies(test, t1, t2, t3, t4, t5, t6) + 1);
            Test.setThingies(test, nextThingy, t1, t2, t3, t4, t5, t6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
