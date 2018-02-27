class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        char nextThingy = 'b';

        System.out.println("Initial nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Initial nextThingy byte value is " + (byte) nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (char) (test.getThingies() + i);
            test.setThingies(nextThingy);
        }

        System.out.println("Final nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Final nextThingy byte value is " + (byte) nextThingy);

    }
}
