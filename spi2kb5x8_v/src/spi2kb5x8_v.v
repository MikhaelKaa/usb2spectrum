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
	//if(SPI_CS == 1'b0) data = { data[38:0], SPI_MOSI };
	data = { data[38:0], SPI_MOSI };
end

wire [4:0]kl_data;
/*assign kl_data[0] = ~((data[39] & ~BA[0]) | (data[38] & ~BA[1]) | (data[37] & ~BA[2]) | (data[36] & ~BA[3]) | (data[35] & ~BA[4]) | (data[34] & ~BA[5]) | (data[33] & ~BA[6]) | (data[32] & ~BA[7]));
assign kl_data[1] = ~((data[31] & ~BA[0]) | (data[30] & ~BA[1]) | (data[29] & ~BA[2]) | (data[28] & ~BA[3]) | (data[27] & ~BA[4]) | (data[26] & ~BA[5]) | (data[25] & ~BA[6]) | (data[24] & ~BA[7]));
assign kl_data[2] = ~((data[23] & ~BA[0]) | (data[22] & ~BA[1]) | (data[21] & ~BA[2]) | (data[20] & ~BA[3]) | (data[19] & ~BA[4]) | (data[18] & ~BA[5]) | (data[17] & ~BA[6]) | (data[16] & ~BA[7]));
assign kl_data[3] = ~((data[15] & ~BA[0]) | (data[14] & ~BA[1]) | (data[13] & ~BA[2]) | (data[12] & ~BA[3]) | (data[11] & ~BA[4]) | (data[10] & ~BA[5]) | (data[9]  & ~BA[6]) | (data[8]  & ~BA[7]));
assign kl_data[4] = ~((data[7 ] & ~BA[0]) | (data[6 ] & ~BA[1]) | (data[5 ] & ~BA[2]) | (data[4]  & ~BA[3]) | (data [3] & ~BA[4]) | (data[2]  & ~BA[5]) | (data[1]  & ~BA[6]) | (data[0]  & ~BA[7]));*/

assign kl_data[0] = ~((data[39] & ~BA[0]) | (data[38] & ~BA[1]) | (data[37] & ~BA[2]) | (data[36] & ~BA[3]) | (data[35] & ~BA[4]) | (data[34] & ~BA[5]) | (data[33] & ~BA[6]) | (data[32] & ~BA[7]));
assign kl_data[1] = ~((data[31] & ~BA[0]) | (data[30] & ~BA[1]) | (data[29] & ~BA[2]) | (data[28] & ~BA[3]) | (data[27] & ~BA[4]) | (data[26] & ~BA[5]) | (data[25] & ~BA[6]) | (data[24] & ~BA[7]));
assign kl_data[2] = ~((data[23] & ~BA[0]) | (data[22] & ~BA[1]) | (data[21] & ~BA[2]) | (data[20] & ~BA[3]) | (data[19] & ~BA[4]) | (data[18] & ~BA[5]) | (data[17] & ~BA[6]) | (data[16] & ~BA[7]));
assign kl_data[3] = ~((data[15] & ~BA[0]) | (data[14] & ~BA[1]) | (data[13] & ~BA[2]) | (data[12] & ~BA[3]) | (data[11] & ~BA[4]) | (data[10] & ~BA[5]) | (data[9]  & ~BA[6]) | (data[8]  & ~BA[7]));
assign kl_data[4] = ~((data[7 ] & ~BA[0]) | (data[6 ] & ~BA[1]) | (data[5 ] & ~BA[2]) | (data[4]  & ~BA[3]) | (data [3] & ~BA[4]) | (data[2]  & ~BA[5]) | (data[1]  & ~BA[6]) | (data[0]  & ~BA[7]));


assign KL = SPI_CS?(kl_data):(5'bz);

endmodule
