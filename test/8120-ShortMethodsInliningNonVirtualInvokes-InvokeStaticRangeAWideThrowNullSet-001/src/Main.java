// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static double[] thingiesArray = new double[iterations];

    public static double getThingies(double[] arr, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        return arr[i];
    }

    public static void setThingies(double[] arr, double newThingy, int i, double d1, double d2, double d3, double d4, double d5, double d6) {
        arr[i] = newThingy;
    }
    
    public static void main(String[] args) {
        double nextThingy = -10.0;
        double sumArrElements = 0.0;
        for(int i = 0; i < iterations; i++) {
            thingiesArray[i] = i*1.0;
            sumArrElements = sumArrElements + thingiesArray[i];
        }
        try
	{
        for(int i = 0; i < iterations; i++) {
            nextThingy = getThingies(thingiesArray, i, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0) - i*1.0;
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
        }
	}
	catch(NullPointerException ex)
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
