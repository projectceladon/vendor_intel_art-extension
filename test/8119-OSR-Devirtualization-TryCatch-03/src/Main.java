
class Main {

    public static int runTest(int n ) {
                CondVirtBase test = new CondVirtExt();
		int result = 0;
       

        // devirtualization inside finally block: OSR + devirtualization is applied
        try {
            if (n > 0) 
            throw new Exception("Exception 1");
        } catch (Exception e) {
            if ( n > 5)
            throw new Exception("Exception 1");
        } finally {
            for (int i = 0; i < 10*1000*1000; i++) {
                result += test.getThingies();
            }
		    return result;
        }
    }

    public static void main(String[] args) {
        int result = runTest(10);
        System.out.println("Result " + result);
    }

}
