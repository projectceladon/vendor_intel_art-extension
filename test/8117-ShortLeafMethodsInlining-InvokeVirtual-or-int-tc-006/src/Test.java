class Test {

    public int simple_method(int jj, int kk) {
        jj--;
        try {
            throw new Exception("Test");
        } catch (Exception e) {
            jj++;
        }
        jj = kk | jj;
        return jj;
    }

}
