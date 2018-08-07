
public class Example2 {
    private static String ID = null;
    private static String substr = null;
    private static int count = 0;
    public Example2 (String str) {
        Example2.ID = str.substring(0, str.length() - 1);
        

    }
    static {
        substr = ID.substring(0, ID.length() - 1 + Example2.count);
    }

    public String getSubstring() {
        Example2.count += 10;
        return substr;
    }
}


