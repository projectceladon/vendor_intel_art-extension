// The test checks that stack after NullPointerException occurs is correct despite inlining
class Main {
    final static int iterations = 10;
    
    public static void main(String[] args) {
        Test test = new Test(iterations);

        char nextThingy = 'a';
        char d1 = 'a';
        char d2 = 'b';
        char d3 = 'c';
        char d4 = 'd';
        char d5 = 'e';
        char d6 = 'f';

        char sumArrElements = '0';
        for(int i = 0; i < iterations; i++) {
            SuperTest.thingiesArray[i] = (char)(nextThingy + 1);
            sumArrElements = (char)(sumArrElements + SuperTest.thingiesArray[i]);
        }
	try
	{
        for(int i = 0; i < iterations; i++) {
            nextThingy = (char)(test.getThingies(SuperTest.thingiesArray, i, d1, d2, d3, d4, d5, d6) + 1);
            if (i == iterations - 1)
                SuperTest.thingiesArray = null;
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
