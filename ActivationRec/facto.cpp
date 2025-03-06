
int factorial(int n)
{
  int nm1, recResult, answer;

  if (n==0)
    answer=1;
  else
  {
    nm1=n-1;
    recResult=factorial(nm1);
    answer = n * recResult;
  }
 
  return answer;
}
