modules:
{
	source:
	{
		binary="modrep_default/libsource.so";	
		mopts=5;
		variables=(
			{name="block_length";value=308;},
			{name="generator";value=0;}
		);
	};

	crc:
	{
		binary="modrep_ofdm/libgen_crc.so";
		mopts=5;
		variables=({name="long_crc";value=24;});
	};	

	coder:
	{
		binary="modrep_ofdm/liblte_encoder.so";	
		mopts=5;
		variables=({name="padding";value=0;});
	};


	modulator:
	{
		binary="modrep_ofdm/libgen_modulator.so";	
		mopts=5;
		variables=({name="modulation";value=1;});
	};

	padding:
	{
		binary="modrep_ofdm/libgen_padding.so";	
		mopts=5;
		variables=(
			{name="data_type";value=0;},
			{name="pre_padding";value=28;},
			{name="post_padding";value=28;},
			{name="nof_packets";value=7;}
		);
	};

	ifft:
	{
		binary="modrep_ofdm/libgen_dft.so";
		mopts=5;
		variables=(
			{name="direction";value=1;},
			{name="mirror";value=1;},
			{name="normalize";value=1;},
			{name="fft_size";value=128;}
		);
	};

	cyclic:
	{
		binary="modrep_ofdm/libgen_cyclic.so";	
		mopts=5;
		variables=(
			{name="ofdm_symbol_sz";value=128;},
			{name="cyclic_prefix_sz";value=9;},
			{name="first_cyclic_prefix_sz";value=10;}
		);
	};

	sink:
	{
		binary="modrep_default/libplp_sink.so";
		mopts=5;
		variables=(
			{name="is_complex";value=1;},
			{name="mode";value=2;},
			{name="fft_size";value=512;}
		);
	};
};

interfaces:
(
	{src=("source",0);dest=("crc",0)},	
	{src="crc";dest="coder"},	/* default port is 0 */
	{src="coder";dest="modulator"},	
	{src="modulator";dest="padding"},	
	{src="padding";dest="ifft"},	
	{src="ifft";dest="cyclic"},	
	{src="cyclic";dest="sink"}	
);
