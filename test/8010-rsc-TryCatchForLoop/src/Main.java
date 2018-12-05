
public class Main {
    public static double testLoop(int m) {
        double res = 1;
        for (int i = 1; i < m; i++) {
            res = res + i * (i+1);
        }
        return res;
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


