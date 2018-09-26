// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        boolean nextThingy = false;
        boolean sumArrElements = false;
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = true;
            sumArrElements = sumArrElements & SuperTest.thingiesArray[i];
        }
        try
	{
        for(int i = 0; i < iterations; i++) {
            nextThingy = test.getThingies(SuperTest.thingiesArray, i) || true;
            if (i == iterations - 1)
                SuperTest.thingiesArray = null;
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
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
