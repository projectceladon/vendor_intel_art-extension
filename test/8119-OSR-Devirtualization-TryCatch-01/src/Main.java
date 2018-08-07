
class Main {

    public static int runTest() {
                CondVirtBase test = new CondVirtExt();
		int result = 0;
       

        // try-catch inside loop, devirtualization inside try catch: OSR + devirtualization is applied
        for (int i = 0; i < 10*1000*1000; i++) {
            try {
                result += test.getThingies();
            } catch (Exception e) {
                System.out.println("Exception");
            }
        }
		return result;
    }

    public static void main(String[] args) {
        int result = runTest();
        System.out.println("Result " + result);
    }

}
