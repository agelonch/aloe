modes = (
	{name="128fft";
	 desc="1.4 MHz LTE..";
	 opts="";},
	{name="256fft";
	 desc="3 MHz LTE..";
	 opts="";},
	{name="512fft";
	 desc="5 MHz LTE..";
	 opts="";}
);

modules:
{

  coder:
  {
    	binary = "libconvcoder.so";
    	mopts = (("128fft",3),("256fft",5),("512fft",8));

	variables={
		mode="128fft",
			((name="rate"; value=0.5;),
         		(name="blocksize"; value=100;)),
		mode="256fft",
			((name="code"; value=0.3;),
         		(name="blocksize"; value=200;)),
		mode="512fft",
			((name="code"; value=0.3;),
         		(name="blocksize"; value=400;)),
	};
  };
  decoder:
  {
    	binary = "libdecoder.so";
    	mopts = (30, 50, 80);
  };
};

interfaces:
(
	{src=("coder",0); dest=("decoder",0); /* optional, default 1 */ mbpts=0.3; /* optional, default 1 */ delay=1}	
);
