
// 1 non temporal move expected

public class Main {                                                                                                                                                   
    final int iterations = 0x40000;


    class C {
        int value = 0;
        public C(int v) {
            value = v;
        }
    }

    public long checkSum(C[] tab, int n) {
        long s = 0;
        for (int i = 0; i < n ; i++) {
            if (tab[i] != null) {
                s = s + tab[i].value;
            }
                
        }
        return s;
    }

    public long testLoop(C[] tab) {

        int i = 0;


        while (i < iterations) {
            tab[i] = tab[iterations/2];
            i++;
        }
 
        i = 10;

        tab[100] = new C(10);

        while (i < iterations) {
            tab[i] = tab[iterations/2];
            i++;
        }

        int k = 0;
        while (k < 2) {
            i = 0;
            while ( i < iterations) {
                tab[i] = tab[iterations/2];
                i++;
            }
            k++;
        }



        return checkSum(tab, iterations) + tab[iterations/2].value;
    }

    public void test()
    {
        C[] tab = new C [iterations];
        C c = new C(100);        
        for (int i = 0; i < iterations; i++) {
            tab[i] = c;
        }
 
        System.out.println(testLoop(tab));
    }

    public static void main(String[] args)
    {
        new Main().test();
    }

}  

