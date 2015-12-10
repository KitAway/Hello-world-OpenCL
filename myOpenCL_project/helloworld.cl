__kernel void hello(void)
{
	int i = get_global_id(0);
	int j = get_global_id(1);
	int k = get_global_id(2);
	if(i>=300 && j>=300 && k>=300)
		printf("Hello World:%d:%d:%d\n",i,j,k);
}
						  