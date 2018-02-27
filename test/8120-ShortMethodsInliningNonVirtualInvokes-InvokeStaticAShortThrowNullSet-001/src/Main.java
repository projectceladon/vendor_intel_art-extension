// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static short[] thingiesArray = new short[iterations];

    public static short getThingies(short[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(short[] arr, short newThingy, int i) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        short nextThingy = -32768;
        short sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = (short) (nextThingy + 1);
            sumArrElements = (short)(sumArrElements + thingiesArray[i]);
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = (short)(getThingies(thingiesArray, i) + 1);
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i);
        }

    }
}
