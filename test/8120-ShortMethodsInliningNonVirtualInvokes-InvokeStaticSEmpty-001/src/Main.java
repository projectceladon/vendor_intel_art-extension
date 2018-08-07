class Main {
    final static int iterations = 10;
    static int thingies = 0;

    public static int getThingies() {
        return thingies;
    }

    public static void setThingies(int newThingies) {
// empty method
    }

    public static void main(String[] args) {
        int nextThingy = -10;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies() + i;
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);
    }
}

