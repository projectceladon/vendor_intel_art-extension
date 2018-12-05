class Main {
    final static int iterations = 10;
    static short thingies = -32768;

    public static short getThingies(short s1, short s2, short s3, short s4, short s5, short s6) {
        return thingies;
    }

    public static void setThingies(short newThingies, short s1, short s2, short s3, short s4, short s5, short s6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        short nextThingy = thingies;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short)(getThingies(t1, t2, t3, t4, t5, t6) + 1);
            setThingies(nextThingy, t1, t2, t3, t4, t5, t6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    }
}
