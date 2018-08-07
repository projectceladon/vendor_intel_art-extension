
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static char runTest() {
        try {
            div(0);
            return ' ';
        } catch (Exception e) {
            // ignore
        } finally {
            CondVirtBase test = new CondVirtExt();
            char result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        char result = runTest();
        System.out.println("Result " + result);
    }

}
