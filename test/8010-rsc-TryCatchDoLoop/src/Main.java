
public class Main {
    public static int testLoop(int m) {
        int sum = 0;
        int i = 0;
        do {
            sum +=  i;
            i++;
        } while (i < m);
        return sum;
    }

    public void $noinline$test() {
		int res = 0;
	
		res += testLoop(50000);
	
		try {
			if (testLoop(40000) > 0)
				throw new Exception("expected");
		} catch (Exception e) {
			res /= testLoop(60000);
		} finally {
			res -= testLoop(30000);
		}
		
		res *= testLoop(70000);
	
		System.out.println(res);
    }

    public static void main(String[] args) {
        new Main().$noinline$test();
    }
}


