// The test checks that stack after ArrayIndexOutOffBouns Exception occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static short[] thingiesArray = new short[iterations];

    public static short getThingies(short[] arr, int i, short s1, short s2, short s3, short s4, short s5, short s) {
        return arr[i];
    }

    public static void setThingies(short[] arr, short newThingy, int i, short s1, short s2, short s3, short s4, short s5, short s) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        short nextThingy = -32768;
        short sumArrElements = 0;
        short t1 = -32768;
        short t2 = -32767;
        short t3 = -32766;
        short t4 = -32765;
        short t5 = -32764;
        short t6 = -32763;

        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = (short) (nextThingy + 1);
            sumArrElements = (short)(sumArrElements + thingiesArray[i]);
        }
        try
	{
        for(int i = 0; i < iterations + 1; i++) {
            if (i < iterations)
                nextThingy = (short)(getThingies(thingiesArray, i, t1, t2, t3, t4, t5, t6) + 1);
            setThingies(thingiesArray, nextThingy, i, t1, t2, t3, t4, t5, t6);
        }
        }
        catch(ArrayIndexOutOfBoundsException ex)
        {
            ex.printStackTrace();
        }
        catch (Exception ex)
        {
          System.out.println("Caught unhandled exception");
          ex.printStackTrace();
        }
    }
}
