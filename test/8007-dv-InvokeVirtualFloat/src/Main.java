
class Main {

    public static float runTest() {
                CondVirtBase test = new CondVirtExt();
		float result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        float result = runTest();
        System.out.println("Result " + result);
    }

}
