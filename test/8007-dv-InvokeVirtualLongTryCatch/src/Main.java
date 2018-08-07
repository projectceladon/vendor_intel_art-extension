
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static long runTest() {
        try {
            div(0);
            return 0L;
        } catch (Exception e) {
            CondVirtBase test = new CondVirtExt();
            long result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        long result = runTest();
        System.out.println("Result " + result);
    }

}
