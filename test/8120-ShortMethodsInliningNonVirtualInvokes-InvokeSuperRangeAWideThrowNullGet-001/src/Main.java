// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        double nextThingy = -10.0;
        double d1 = 1.0;
        double d2 = 2.0;
        double d3 = 3.0;
        double d4 = 4.0;
        double d5 = 5.0;
        double d6 = 6.0;

        double sumArrElements = 0.0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = i;
            sumArrElements = sumArrElements + SuperTest.thingiesArray[i];
        }
	try
	{
        for(int i = 0; i < iterations; i++) {
            if (i == iterations - 1) 
                SuperTest.thingiesArray = null;
            nextThingy = test.getThingies(SuperTest.thingiesArray, i, d1, d2, d3, d4, d5, d6) - i*1.0;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, d1, d2, d3, d4, d5, d6);
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
