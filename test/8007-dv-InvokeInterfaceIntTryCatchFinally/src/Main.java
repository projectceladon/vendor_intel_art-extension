
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static int runTest() {
        try {
            return div(0);
        } catch (Exception e) {
            // ignore
        } finally {
            CondVirtBase test = new CondVirtExt();
            int result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        int result = runTest();
        System.out.println("Result " + result);
    }

}
