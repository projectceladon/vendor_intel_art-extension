class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        byte nextThingy = -128;

        System.out.println("Initial nextThingy value is " + nextThingy);
        System.out.println("Initial test.thingies value is " + test.thingies);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (byte)(test.gimme() + i);
            test.hereyouare(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
        System.out.println("Final test.thingies value is " + test.thingies);

    } 
}
