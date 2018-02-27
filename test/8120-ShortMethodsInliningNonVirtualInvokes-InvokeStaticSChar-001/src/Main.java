class Main {
    final static int iterations = 10;
    static char thingies = 'a';

    public static char getThingies() {
        return thingies;
    }

    public static void setThingies(char newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        char nextThingy = 'b';

        System.out.println("Initial nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Initial nextThingy byte value is " + (byte) nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (char)(getThingies() + i);
            setThingies(nextThingy);
//        System.out.println( i + " nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
//        System.out.println( i + " nextThingy byte value is " + (byte) nextThingy);

        }

        System.out.println("Final nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Final nextThingy byte value is " + (byte) nextThingy);

    }
}
