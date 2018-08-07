// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static Foo[] thingiesArray = new Foo[iterations];

    public static Foo getThingies(Foo[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(Foo[] arr, Foo newThingy, int i) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        Foo nextThingy = new Foo(-1);

        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = new Foo(i);
        }
        
        try {
            for(int i = 0; i < iterations + 1; i++) {
               if (i == iterations) 
                  thingiesArray = null;
               nextThingy = getThingies(thingiesArray, i);
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
