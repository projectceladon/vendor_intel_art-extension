
class Main {

    public static double runTest() {
                CondVirtBase test = new CondVirtExt();
		double result = test.getThingies();
		return result;
    }

    public static void main(String[] args) {
        double result = runTest();
        System.out.println("Result " + result);
    }

}
