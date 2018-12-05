class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        byte nextThingy = -128;
        byte c1 = -1;
        byte c2 = -2;
        byte c3 = -3;
        byte c4 = -4;
        byte c5 = -5;
        byte c6 = -6;

        System.out.println("Initial nextThingy value is " + nextThingy);
        System.out.println("Initial test.thingies value is " + test.thingies);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (byte)(test.gimme(c1, c2, c3, c4, c5, c6) + i);
            test.hereyouare(nextThingy, c1, c2, c3, c4, c5, c6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
        System.out.println("Final test.thingies value is " + test.thingies);

    } 
}
