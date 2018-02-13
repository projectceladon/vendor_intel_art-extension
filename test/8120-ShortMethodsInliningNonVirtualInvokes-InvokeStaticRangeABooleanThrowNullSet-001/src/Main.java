// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static boolean[] thingiesArray = new boolean[iterations];

    public static boolean getThingies(boolean[] arr, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        return arr[i];
    }

    public static void setThingies(boolean[] arr, boolean newThingy, int i, int i1, int i2, int i3, int i4, int i5, int i6) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        boolean nextThingy = false;
        boolean sumArrElements = false;
        int j1, j2, j3, j4, j5, j6;
        j1 = 1;
        j2 = 2;
        j3 = 3;
        j4 = 4;
        j5 = 5;
        j6 = 6;

        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = true;
            sumArrElements = sumArrElements & thingiesArray[i];
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(thingiesArray, i, j1, j2, j3, j4, j5, j6) || true;
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i, j1, j2, j3, j4, j5, j6);
        }

    }
}
