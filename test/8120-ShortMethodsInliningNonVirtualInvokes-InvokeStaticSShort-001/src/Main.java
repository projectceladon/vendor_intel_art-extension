class Main {
    final static int iterations = 10;
    static short thingies = -32768;

    public static short getThingies() {
        return thingies;
    }

    public static void setThingies(short newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        short nextThingy = thingies;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short)(getThingies() + 1);
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
