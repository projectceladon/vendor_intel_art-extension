class Main {
    final  static int iterations = 10;
    static boolean thingies = false;

    public static boolean getThingies(int i1, int i2, int i3, int i4, int i5, int i6) {
        return thingies;
    }

    public static void setThingies(boolean newThingies, int i1, int i2, int i3, int i4, int i5, int i6) {
        thingies = newThingies;
    }
    
    public static void main(String[] args) {
        boolean nextThingy = false;
        int j1, j2, j3, j4, j5, j6;
        j1 = 1;
        j2 = 2;
        j3 = 3;
        j4 = 4;
        j5 = 5;
        j6 = 6;

        System.out.println("Initial nextThingy value is " + nextThingy);

        for(int i = 0; i < iterations; i++) {
            nextThingy = !getThingies(j1, j2, j3, j4, j5, j6);
            setThingies(nextThingy, j1, j2, j3, j4, j5, j6);
        }

        System.out.println("Final nextThingy value is " + nextThingy);

    } 
}
