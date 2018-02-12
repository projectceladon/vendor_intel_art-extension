
class Main {

    public static int runTest(int n ) {
        CondVirtBase test = new CondVirtExt();
        int result = 0;


        // try-catch before and after OSR candidate loop: OSR + devirtualization is applied
        // nested loops
        try {
            if (n > 0) 
                throw new Exception("Exception 1");
        } catch (Exception e) {
            System.out.println(e);
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < 10*100*1000; k++) {
                    result += test.getThingies();
                }
                for (int k = 0; k < 10*100*1000; k++) {
                    result += test.getThingies();
                }

            }
        }
        try {
            if (n > 0) 
                throw new Exception("Exception 2");
        } catch (Exception e) {
            System.out.println(e);
        }

        return result;
    }

    public static void main(String[] args) {
        int result = runTest(5);
        System.out.println("Result " + result);
    }

}
