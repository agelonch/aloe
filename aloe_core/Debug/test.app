modes = (
	{name="128fft";
	 desc="1.4 MHz LTE.."},
	{name="256fft";
	 desc="3 MHz LTE..";}
);

modules:
{

  tx:
  {
    	binary = "test_tx";
    	mopts = (("128fft",3),("256fft",5));

	variables=(
		{name="rate"; value=(("128fft",0.5),("256fft",0.3));},
		{name="length"; value=(("128fft",100),("256fft",200));}
	);
  };
  rx:
  {
    	binary = "test_rx";
    	mopts = 30.3;
    
    	variables=({name="hola";value=1;});
  };
};

interfaces:
(
	{src=("tx",0); dest=("rx",0); /* optional, default 1 */ mbpts=0.3; /* optional, default 1 */ delay=1}	
);
