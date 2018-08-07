class Main {
    final static int iterations = 10;
    static double thingies = 0;

    public static double getThingies() {
        return thingies;
    }

    public static void setThingies(double newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        double nextThingy = -100000.00;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies() + i*10.00;
            setThingies(nextThingy);
        }
        
         System.out.println("Final nextThingy value is " + nextThingy);

    }
}
