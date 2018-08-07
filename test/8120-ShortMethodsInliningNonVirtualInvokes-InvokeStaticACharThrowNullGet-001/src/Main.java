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

        try {
            for(int i = 0; i < iterations + 1; i++) {
               if (i == iterations) 
                  thingiesArray = null;
               nextThingy = (char)(getThingies(thingiesArray, i) + 1);
               setThingies(thingiesArray, nextThingy, i);
            }
        } catch (NullPointerException ex) {
            ex.printStackTrace();
        } catch (Exception ex) {
            System.out.println("Caught unhandled exception");
            ex.printStackTrace();
        }

    }
}
