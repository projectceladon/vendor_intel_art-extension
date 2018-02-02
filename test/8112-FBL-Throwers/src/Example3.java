
public class Example3 {
    private static String ID = null;
    private static String substr = null;
    private static int count = 0;
    public Example3 (String str) {
        Example3.ID = str.substring(0, str.length() - 1);
        

    }
    static {
        substr = ID.substring(0, ID.length() - 1 + Example3.count);
    }

    public String getSubstring() {
        Example3.count += 10;
        return substr;
    }
}


