
class Main {

    public static int div(int d) {
        return 1 / d;
    }

    public static byte runTest() {
        try {
            div(0);
            return (byte)0;
        } catch (Exception e) {
            // ignore
        } finally {
            CondVirtBase test = new CondVirtExt();
            byte result = test.getThingies();
		    return result;
        }
    }

    public static void main(String[] args) {
        byte result = runTest();
        System.out.println("Result " + result);
    }

}
