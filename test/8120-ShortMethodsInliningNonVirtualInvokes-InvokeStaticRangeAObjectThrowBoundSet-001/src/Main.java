// The test checks that stack after ArrayIndexOutOffBouns Exception occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static Foo[] thingiesArray = new Foo[iterations];

    public static Foo getThingies(Foo[] arr, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        return arr[i];
    }

    public static void setThingies(Foo[] arr, Foo newThingy, int i, Foo f1, Foo f2, Foo f3, Foo f4, Foo f5, Foo f6) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        Foo nextThingy = new Foo();
        Foo g1 = new Foo();
        Foo g2 = new Foo();
        Foo g3 = new Foo();
        Foo g4 = new Foo();
        Foo g5 = new Foo();
        Foo g6 = new Foo();

        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = new Foo();
        }

        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = getThingies(thingiesArray, i, g1, g2, g3, g4, g5, g6);
            setThingies(thingiesArray, nextThingy, i, g1, g2, g3, g4, g5, g6);
        }

    }
}
