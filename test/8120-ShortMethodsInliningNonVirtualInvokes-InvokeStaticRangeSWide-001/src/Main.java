class Main {
    final static int iterations = 10;
    static double thingies = 0;

    public static double getThingies(double d1, double d2, double d3, double d4, double d5, double d6) {
        return thingies;
    }

    public static void setThingies(double newThingies, double d1, double d2, double d3, double d4, double d5, double d6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        double nextThingy = -100000.00;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(1.0, 2.0, 3.0, 4.0, 5.0, 6.0) + i*10.00;
            setThingies(nextThingy, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        }

         System.out.println("Final nextThingy value is " + nextThingy);

    }
}
