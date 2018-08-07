
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static short runTest() {
        try {
            div(0);
            return (short)0;
        } catch (Exception e) {
            CondVirtBase test = new CondVirtExt();
            short result = test.getThingies();
            return result;
        }
    }

    public static void main(String[] args) {
        short result = runTest();
        System.out.println("Result " + result);
    }

}
