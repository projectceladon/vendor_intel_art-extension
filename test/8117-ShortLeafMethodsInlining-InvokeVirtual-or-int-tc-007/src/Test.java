class Test {

    public int simple_method(int jj, int kk) throws Exception {
        if (kk % 2 == 0)
            throw new Exception("Test");
        jj = kk | jj;
        return jj;
    }

    public int simple_1method(int jj, int kk) {
        jj = kk | jj;
        return jj;
    }

    public int simple_2method(int jj, int kk) throws Exception {
        throw new Exception("Test");
    }

}
