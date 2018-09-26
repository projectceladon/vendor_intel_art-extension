
class Main {

    public static short runTest() {
                CondVirtBase test = new CondVirtExt();
		short result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        short result = runTest();
        System.out.println("Result " + result);
    }

}
