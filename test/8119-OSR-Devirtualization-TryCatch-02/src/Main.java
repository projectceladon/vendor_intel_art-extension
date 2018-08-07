
class Main {

    public static int runTest() {
                CondVirtBase test = new CondVirtExt();
		int result = 0;
       

        // devirtualization inside catch block: OSR + devirtualization is applied
        try {
            throw new Exception("My custom exception");
        } catch (Exception e) {
            System.out.println("Exception");
            for (int i = 0; i < 10*1000*1000; i++) {
                result += test.getThingies();
            }
        }
		return result;
    }

    public static void main(String[] args) {
        int result = runTest();
        System.out.println("Result " + result);
    }

}
