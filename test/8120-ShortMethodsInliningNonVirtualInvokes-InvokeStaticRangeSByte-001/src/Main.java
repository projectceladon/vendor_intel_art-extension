class Main {
    final static int iterations = 10;
    static byte thingies = -128;

    public static byte getThingies(byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        return thingies;
    }

    public static void setThingies(byte newThingies, byte b1, byte b2, byte b3, byte b4, byte b5, byte b6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        byte nextThingy = thingies;
        byte c1 = -1;
        byte c2 = -2;
        byte c3 = -3;
        byte c4 = -4;
        byte c5 = -5;
        byte c6 = -6;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (byte)(getThingies(c1, c2, c3, c4, c5, c6) + i);
            setThingies(nextThingy, c1, c2, c3, c4, c5, c6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
