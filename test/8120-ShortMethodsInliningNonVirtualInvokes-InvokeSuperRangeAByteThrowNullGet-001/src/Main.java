// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        byte nextThingy = -128;
        byte c1 = -1;
        byte c2 = -2;
        byte c3 = -3;
        byte c4 = -4;
        byte c5 = -5;
        byte c6 = -6;

        byte sumArrElements = 0;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (byte)(nextThingy + 1);
            sumArrElements = (byte)(sumArrElements + SuperTest.thingiesArray[i]);
        }
	try
	{
        for(int i = 0; i < iterations; i++) {
            if (i == iterations - 1) 
                SuperTest.thingiesArray = null;
            nextThingy = (byte)(test.getThingies(SuperTest.thingiesArray, i, c1, c2, c3, c4, c5, c6) + 1);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i, c1, c2, c3, c4, c5, c6);
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
