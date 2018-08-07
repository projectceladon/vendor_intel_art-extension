class Main {
    final static int iterations = 10;
    static int[] thingiesArray = new int[iterations];

//    public static int getThingies(int i) {
//        return thingiesArray[i];
//    }
//  |[000194] Main.getThingies:(I)I
//  |0000: sget-object v0, LMain;.thingiesArray:[I // field@0001
//  |0002: aget v0, v0, v1
//  |0004: return v0


    public static int getThingies(int[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(int[] arr, int newThingy, int i) {
// empty method
    }
    
    public static void main(String[] args) {
        int nextThingy = -10;
        int sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = i;
            sumArrElements = sumArrElements + thingiesArray[i];
        }

        System.out.println("Initial sumArrElements = " + sumArrElements);

        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(thingiesArray, i) - i*1;
            setThingies(thingiesArray, nextThingy, i);
        }

        sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            sumArrElements = sumArrElements + thingiesArray[i];
        }

        System.out.println("Final sumArrElements = " + sumArrElements);

    }
}
