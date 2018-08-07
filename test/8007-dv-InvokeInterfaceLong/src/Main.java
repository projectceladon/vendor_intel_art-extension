
class Main {

    public static long runTest() {
                CondVirtBase test = new CondVirtExt();
		long result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        long result = runTest();
        System.out.println("Result " + result);
    }

}
