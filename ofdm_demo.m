addpath('/usr/local/mex')

Ncycles=20;

crc_params={{'long_crc',int32(24)}};
coder_params={{'padding',int32(0)}};
modulator_params={{'modulation',int32(1)}};
padding_params={{'data_type',int32(0)},{'pre_padding',int32(28)},{'post_padding',int32(28)},{'nof_packets',int32(7)}};
ifft_params={{'direction',int32(1)},{'mirror',int32(1)},{'normalize',int32(1)},{'fft_size',int32(128)}};
cyclic_params={{'ofdm_symbol_sz',int32(128)},{'cyclic_prefix_sz',int32(9)},{'first_cyclic_prefix_sz',int32(10)}};

output=[];
for i=1:Ncycles
	in_bits = randi(2,308,1)==1;
	
	out_crc = am_gen_crc(in_bits,crc_params);	

	out_encoder = am_lte_encoder(out_crc,coder_params);	

	out_modulator = am_gen_modulator(out_encoder,modulator_params);	

	out_padding = am_gen_padding(out_modulator,padding_params);	

	out_ifft = am_gen_dft(out_padding,ifft_params);	

	out_cyclic = am_gen_cyclic(out_ifft,cyclic_params);	

	output = [output out_cyclic];
end

plot(10*log10(pwelch(output)));
