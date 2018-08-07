
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static float runTest() {
        try {
            div(0);
            return 0.0f;
        } catch (Exception e) {
            // ignore
        } finally {
            CondVirtBase test = new CondVirtExt();
            float result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        float result = runTest();
        System.out.println("Result " + result);
    }

}
