int facto(int a) {
	printf("&a=%d\n", (int)&a);
	p();
	int b = a-1;
	if (!a)
	{
		return 1;
	} else {
		return a*facto(b);
	}
}
