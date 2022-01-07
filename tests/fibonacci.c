int fib(int n)
{
    if (n <= 1){
        return n;
    }

    return fib(n - 1) + fib(n - 2);
}
  
int main()
{
    int n = 9231;
    int result = fib(2);
    return 0;
}