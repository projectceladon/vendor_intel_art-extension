class Main {
    final static int iterations = 10;
    static int thingies = 0;

    public static int getThingies(char c1, char c2, char c3, char c4, char c5, char c6) {
        return thingies;
    }

    public static void setThingies(int newThingies, char c1, char c2, char c3, char c4, char c5, char c6) {
// emplty method
    }

    public static void main(String[] args) {
        int nextThingy = -10;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies('a', 'b', 'c', 'd', 'e', 'f') + i;
            setThingies(nextThingy, 'a', 'b', 'c', 'd', 'e', 'f');
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}

