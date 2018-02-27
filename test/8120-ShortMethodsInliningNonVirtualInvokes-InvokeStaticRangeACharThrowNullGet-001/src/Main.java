// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static char[] thingiesArray = new char[iterations];

    public static int getThingies(char[] arr, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        return arr[i];
    }

    public static void setThingies(char[] arr, char newThingy, int i, char c1, char c2, char c3, char c4, char c5, char c6) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        char nextThingy = 'a';
        char sumArrElements = '0';
        char d1 = 'a';
        char d2 = 'b';
        char d3 = 'c';
        char d4 = 'd';
        char d5 = 'e';
        char d6 = 'f';

        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = (char) (nextThingy + 1);
            sumArrElements = (char)(sumArrElements + thingiesArray[i]);
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i == iterations) 
                thingiesArray = null;
            nextThingy = (char)(getThingies(thingiesArray, i, d1, d2, d3, d4, d5, d6) + 1);
            setThingies(thingiesArray, nextThingy, i, d1, d2, d3, d4, d5, d6);
        }

    }
}
