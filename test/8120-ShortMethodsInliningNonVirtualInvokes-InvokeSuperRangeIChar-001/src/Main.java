class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test();
        char nextThingy = 'b';
        char d1 = 'a';
        char d2 = 'b';
        char d3 = 'c';
        char d4 = 'd';
        char d5 = 'e';
        char d6 = 'f';

        System.out.println("Initial nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Initial nextThingy byte value is " + (byte) nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (char) (test.getThingies(d1, d2, d3, d4, d5, d6) + i);
            test.setThingies(nextThingy, d1, d2, d3, d4, d5, d6);
        }

        System.out.println("Final nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Final nextThingy byte value is " + (byte) nextThingy);

    }
}
