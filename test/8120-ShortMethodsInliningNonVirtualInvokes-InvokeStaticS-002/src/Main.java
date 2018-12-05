class Main {
    final static int iterations = 10;
    static float thingies = 0;

    public static float getThingies() {
        return thingies;
    }

    public static void setThingies(float newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        float nextThingy = -100000.00f;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies() + i*10.00f;
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
