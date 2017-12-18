
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static double runTest() {
        try {
            div(0);
            return 0.0d;
        } catch (Exception e) {
            // ignore
        } finally {
            CondVirtBase test = new CondVirtExt();
            double result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        double result = runTest();
        System.out.println("Result " + result);
    }

}
