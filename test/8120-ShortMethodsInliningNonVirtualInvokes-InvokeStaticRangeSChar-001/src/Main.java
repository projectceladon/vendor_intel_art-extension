class Main {
    final static int iterations = 10;
    static char thingies = 'a';

    public static char getThingies(char c1, char c2, char c3, char c4, char c5, char c6) {
        return thingies;
    }

    public static void setThingies(char newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
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
            nextThingy = (char)(getThingies(d1, d2, d3, d4, d5, d6) + i);
            setThingies(nextThingy, d1, d2, d3, d4, d5, d6);
        }

        System.out.println("Final nextThingy unicode value is " + String.format ("\\u%04x", (int)nextThingy));
        System.out.println("Final nextThingy byte value is " + (byte) nextThingy);

    }
}
