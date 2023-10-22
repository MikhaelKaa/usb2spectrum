module spi2kb5x8_v (

// 
input [7:0]BA,

//
output [4:0] KL,

//
input SPI_CLK,
input SPI_CS,
input SPI_MOSI
);

// SPI receive
reg [39:0] data = 40'b0;

always @(negedge SPI_CLK) begin
	data = { data[38:0], SPI_MOSI };
end

// I should do it like this, but I'm too lazy.
/*wire RESET = 1'b1;
always @(negedge SPI_CLK or negedge RESET) begin
	if(!RESET) begin
		if(SPI_CS == 1'b0) begin 
			data = { data[38:0], SPI_MOSI };
		end
	end
	else begin
		data = 40'b0;
	end
	
end*/

// 
wire [4:0]kl_data;
assign kl_data[4] = ~((data[39] & ~BA[7]) | (data[38] & ~BA[6]) | (data[37] & ~BA[5]) | (data[36] & ~BA[4]) | (data[35] & ~BA[3]) | (data[34] & ~BA[2]) | (data[33] & ~BA[1]) | (data[32] & ~BA[0]));
assign kl_data[3] = ~((data[31] & ~BA[7]) | (data[30] & ~BA[6]) | (data[29] & ~BA[5]) | (data[28] & ~BA[4]) | (data[27] & ~BA[3]) | (data[26] & ~BA[2]) | (data[25] & ~BA[1]) | (data[24] & ~BA[0]));
assign kl_data[2] = ~((data[23] & ~BA[7]) | (data[22] & ~BA[6]) | (data[21] & ~BA[5]) | (data[20] & ~BA[4]) | (data[19] & ~BA[3]) | (data[18] & ~BA[2]) | (data[17] & ~BA[1]) | (data[16] & ~BA[0]));
assign kl_data[1] = ~((data[15] & ~BA[7]) | (data[14] & ~BA[6]) | (data[13] & ~BA[5]) | (data[12] & ~BA[4]) | (data[11] & ~BA[3]) | (data[10] & ~BA[2]) | (data[9]  & ~BA[1]) | (data[8]  & ~BA[0]));
assign kl_data[0] = ~((data[7 ] & ~BA[7]) | (data[6 ] & ~BA[6]) | (data[5 ] & ~BA[5]) | (data[4]  & ~BA[4]) | (data [3] & ~BA[3]) | (data[2]  & ~BA[2]) | (data[1]  & ~BA[1]) | (data[0]  & ~BA[0]));

// 
assign KL = SPI_CS?(kl_data):(5'bz);

endmodule
