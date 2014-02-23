extern "C" {
 void init(void); // TODO: figure out why init.cpp got compiled with C linkage (no name mangling)
 void setup(void);
 void loop(void);
}
int main( void )
{
	init();

	setup();

	for (;;)
	{
		loop();
	}

	return 0;
}
