// The test checks that stack after ArrayIndexOutOfBoundsException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        char nextThingy = 'a';
        char sumArrElements = '0';
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (char)(nextThingy + 1);
            sumArrElements = (char)(sumArrElements + SuperTest.thingiesArray[i]);
        }
        try
	{
        for(int i = 0; i < iterations + 1; i++) {
            nextThingy = (char)(test.getThingies(SuperTest.thingiesArray, i) + 1);
            test.setThingies(SuperTest.thingiesArray, nextThingy, i);
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
