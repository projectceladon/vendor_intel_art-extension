// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static float[] thingiesArray = new float[iterations];

    public static float getThingies(float[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(float[] arr, float newThingy, int i) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        float nextThingy = -10.0f;
        float sumArrElements = 0.0f;
        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = i*1.0f;
            sumArrElements = sumArrElements + thingiesArray[i];
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(thingiesArray, i) - i*1.0f;
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i);
        }
    }
}
