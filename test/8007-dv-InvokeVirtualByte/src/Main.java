
class Main {

    public static byte runTest() {
                CondVirtBase test = new CondVirtExt();
		byte result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        byte result = runTest();
        System.out.println("Result " + result);
    }

}
