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

  };
  mut:
  {
    	binary = "testpp";
    	mopts = 5;

		variables=({name="block_length"; value=128;});
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
	{src=("tx",0); dest=("mut",0); /* optional, default 1 */ mbpts=0.3; /* optional, default 1 */ delay=1},	
	{src=("mut",0); dest=("rx",0);}	
);
