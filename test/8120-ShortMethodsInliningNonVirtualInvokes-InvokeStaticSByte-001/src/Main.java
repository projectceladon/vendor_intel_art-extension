class Main {
    final static int iterations = 10;
    static byte thingies = -128;

    public static byte getThingies() {
        return thingies;
    }

    public static void setThingies(byte newThingies) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        byte nextThingy = thingies;
 
        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (byte)(getThingies() + i);
            setThingies(nextThingy);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
