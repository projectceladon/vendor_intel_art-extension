
class Main {

    public static int runTest() {
                CondVirtBase test = new CondVirtExt();
		int result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        int result = runTest();
        System.out.println("Result " + result);
    }

}
