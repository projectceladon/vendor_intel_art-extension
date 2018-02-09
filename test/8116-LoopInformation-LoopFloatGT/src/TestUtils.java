import java.lang.reflect.Method;


public class TestUtils {

    static public void runTests(Object test, Object... args) {
        Method[] methods = test.getClass().getDeclaredMethods();
        for (int i = 1; i < methods.length; i++) {
            int j = i;
            while (j > 0 && methods[j - 1].getName().compareTo(methods[j].getName()) > 0) {
                Method tempMethod = methods[j];
                methods[j] = methods[j - 1];
                methods[j - 1] = tempMethod;
                j--;
            }
        }
        for (Method method : methods) {
            if (method.getName().startsWith("test")) {
                try {
                    String names[] = test.getClass().getName().split("\\.");
                    String testName = names[names.length - 1];
                    System.out.println(testName + "." + method.getName() + " result: " + method.invoke(test, args));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public static int hash(float... fs) {
        return hash$noinline$(fs);
    }

    private static int hash$noinline$(float... fs) {
        int hash = 0;
        for (float f : fs) {
            //System.out.println("hash: " + f + " -> " + Float.floatToIntBits(f));
            hash += Float.floatToIntBits(f);
        }
        return hash;
    }

}
