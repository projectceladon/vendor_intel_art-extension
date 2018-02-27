// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static char[] thingiesArray = new char[iterations];

    public static int getThingies(char[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(char[] arr, char newThingy, int i) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        char nextThingy = 'a';
        char sumArrElements = '0';
        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = (char) (nextThingy + 1);
            sumArrElements = (char)(sumArrElements + thingiesArray[i]);
        }

        for(int i = 0; i < iterations; i++) {
            nextThingy = (char)(getThingies(thingiesArray, i) + 1);
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i);
        }

    }
}
