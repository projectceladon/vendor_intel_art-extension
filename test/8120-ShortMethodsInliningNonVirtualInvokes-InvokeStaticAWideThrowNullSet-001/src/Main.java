// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    static double[] thingiesArray = new double[iterations];

    public static double getThingies(double[] arr, int i) {
        return arr[i];
    }

    public static void setThingies(double[] arr, double newThingy, int i) {
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
            nextThingy = getThingies(thingiesArray, i) - i*1.0;
            if (i == iterations - 1)
                thingiesArray = null;
            setThingies(thingiesArray, nextThingy, i);
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
