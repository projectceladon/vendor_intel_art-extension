class Main {
    final static int iterations = 10;
    static boolean thingies = false;

    public static boolean getThingies() {
        return thingies;
    }

    public static void setThingies(boolean newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        boolean nextThingy = false;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = !getThingies();
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    } 
}
