
class Main {

    public static char runTest() {
                CondVirtBase test = new CondVirtExt();
		char result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        char result = runTest();
        System.out.println("Result " + result);
    }

}
