module xtdata_gate(xtout,xtin,en);
  parameter size=32; //vhdl generic
  output [size-1:0] xtout;
  input [size-1:0] xtin;
  input en;
  
  assign xtout = {size{en}} & xtin;

endmodule // xtdata_gate

module xtdata_gate_mux(xtout,xtin,xtin_prev,en);
  parameter size=32; //vhdl generic
  output [size-1:0] xtout;
  input [size-1:0] xtin;
  input [size-1:0] xtin_prev;
  input en;
  
  assign xtout = en ? xtin : xtin_prev;

endmodule // xtdata_gate_mux

module xtRFenlatch(xtRFenlatchout,xtin,xten,clk);
   parameter size = 32; // vhdl generic
   output [size-1:0] xtRFenlatchout;
   input [size-1:0]  xtin;
   input 	     xten;
   input 	     clk;

   reg [size-1:0]    xtRFenlatchout;
 always @(clk or xten or xtin or xtRFenlatchout) begin
   if (clk)
      xtRFenlatchout <= #1 (xten) ? xtin : xtRFenlatchout;
 end

endmodule

module xtRFlatch(xtRFlatchout,xtin,clk);
   parameter size = 32; // vhdl generic
   output [size-1:0] xtRFlatchout;
   input [size-1:0]  xtin;
   input 	     clk;

   reg [size-1:0]    xtRFlatchout;
 always @(clk or xtin) begin
   if (clk)
      xtRFlatchout <= #1 xtin;
 end

endmodule

module xtadd(xtout, a, b);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  a;
   input [size-1:0]  b;

   assign xtout = a + b;
endmodule

module xtaddc(sum, carry, a, b, c);
   parameter size = 32; // vhdl generic

   output [size-1:0] sum;
   output            carry;
   input [size-1:0]  a;
   input [size-1:0]  b;
   input             c;

   assign {carry,sum} = a + b + c;
endmodule

module xtaddcin(xtout, a, b, c);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  a;
   input [size-1:0]  b;
   input             c;

   assign xtout = a + b + c;
endmodule

module xtaddcout(sum, carry, a, b);
   parameter size = 1; // vhdl generic
 
   output [size-1:0] sum;
   output            carry;
   input [size-1:0]  a;
   input [size-1:0]  b;

   assign {carry,sum} = a + b;
endmodule

module xtbooth(xtout, cin, a, b, sign, negate);
parameter size = 16; // vhdl generic
output [size+1:0] xtout;
output cin;
input [size-1:0] a;
input [2:0] b;
input sign, negate;
    wire ase = sign & a[size-1];
    wire [size+1:0] ax1 = {ase, ase, a};
    wire [size+1:0] ax2 = {ase, a, 1'd0}; //'
    wire one = b[1] ^ b[0];
    wire two = b[2] ? ~b[1] & ~b[0] : b[1] & b[0];
    wire cin = negate ? (~b[2] & (b[1] | b[0])) : (b[2] & ~(b[1] & b[0]));
    wire [size+1:0] size_one = {size+2{one}};
    wire [size+1:0] size_cin = {size+2{cin}};
    wire [size+1:0] size_two = {size+2{two}};
    assign xtout = size_cin ^ (ax1 & size_one | ax2 & size_two);
endmodule

module xtcompare(xtout, a, b);
   parameter size = 32; // vhdl generic

   output 	    xtout;
   input [size-1:0] a;
   input [size-1:0] b;

   assign xtout = (a == b);

endmodule // xtcompare

module xtcsa (sum, carry, a, b, c);
   parameter size = 1; // vhdl generic

   output [size-1:0]	sum;
   output [size-1:0]	carry;
   input [size-1:0]	a;
   input [size-1:0]	b;
   input [size-1:0]	c;
   
   assign sum = a ^ b ^ c;
   assign carry = (a & b) | (b & c) | (c & a) ;

endmodule

module xtascflop(xtout, xtin, clrb, clk);       // async clear
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input             clrb;
   input           clk;
   reg [size-1:0]    tmp;

   // synopsys async_set_reset "clrb"
   // cadence async_set_reset "clrb"

   assign xtout = tmp;
   always @(posedge clk or negedge clrb) begin
      if (!clrb) tmp <= #1 {size{1'b0}};
      else tmp <= #1 xtin;
   end
endmodule // xtascflop

module xtaspflop(xtout, xtin, presetb, clk);       // async preset
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input             presetb;
   input           clk;
   reg [size-1:0]    tmp;

   // synopsys async_set_reset "presetb"
   // cadence async_set_reset "presetb"

   assign xtout = tmp;
   always @(posedge clk or negedge presetb) begin
      if (!presetb) tmp <= #1 {size{1'b1}};
      else tmp <= #1 xtin;
   end
endmodule // xtaspflop

module xtascenflop(xtout, xtin, en, clrb, clk);       // async clear
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input           en;
   input             clrb;
   input           clk;
   reg [size-1:0]    tmp;

   // synopsys async_set_reset "clrb"
   // cadence async_set_reset "clrb"

   assign xtout = tmp;
   always @(posedge clk or negedge clrb) begin
      if (!clrb) tmp <= #1 {size{1'b0}};
      else if (en)
      tmp <= #1 xtin;
   end
endmodule // xtascenflop

module xtaspenflop(xtout, xtin, en, presetb, clk);       // async preset
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input           en;
   input             presetb;
   input           clk;
   reg [size-1:0]    tmp;

   // synopsys async_set_reset "presetb"
   // cadence async_set_reset "presetb"

   assign xtout = tmp;
   always @(posedge clk or negedge presetb) begin
      if (!presetb) tmp <=  #1 {size{1'b1}};
      else if (en)
      tmp <= #1 xtin;
   end
endmodule // xtaspenflop

module xtassflop(xtout, xtin, setb, xtsval, clk);	// async set
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input [size-1:0]  xtsval;
   input             setb;
   input 	     clk;
   wire [size-1:0]    tmp_int;

   // synopsys async_set_reset "setb"
   // cadence async_set_reset "setb"

   wire [size-1:0]    tmp = tmp_int ^ xtsval;
   wire [size-1:0]    xtin_int = xtin ^ xtsval;
   xtascflop #(size) acflop_i (tmp_int, xtin_int, setb, clk);
   assign xtout = tmp;
endmodule 

module xtassenflop(xtout, xtin, en, setb, xtsval, clk);	// async set
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input [size-1:0]  xtsval;
   input 	     en;
   input             setb;
   input 	     clk;
   wire [size-1:0]    tmp_int;

   // synopsys async_set_reset "setb"
   // cadence async_set_reset "setb"

   wire [size-1:0]    tmp = tmp_int ^ xtsval;
   wire [size-1:0]    xtin_int = xtin ^ xtsval;
   xtascenflop #(size) acenflop_i (tmp_int, xtin_int, en, setb, clk);
   assign xtout = tmp;
endmodule

module xtenflop(xtout, xtin, en, clk);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input 	     en;
   input 	     clk;
   reg [size-1:0]    tmp;
   assign xtout = tmp;
   always @(posedge clk) begin
         tmp <= #1 (en) ? xtin : tmp;
   end

endmodule

module xtnenflop(xtout, xtin, en, clk);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input 	     en;
   input 	     clk;
   reg [size-1:0]    tmp;
   assign xtout = tmp;
   always @(negedge clk) begin
         tmp <= #1 (en) ? xtin : tmp;
   end

endmodule

module xtfa(sum, carry, a, b, c);
output sum, carry;
input a, b, c;
    assign sum = a ^ b ^ c;
    assign carry = a & b | a & c | b & c;
endmodule

module xtflop(xtout, xtin, clk);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input 	     clk;
   reg [size-1:0]    tmp;
   assign xtout = tmp;
   always @(posedge clk) begin
         tmp <= #1 xtin;
   end

endmodule

module xtha(sum, carry, a, b);
output sum, carry;
input a, b;
    assign sum = a ^ b;
    assign carry = a & b;
endmodule

module xtinc(xtout, a);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  a;
   
   assign xtout = a + 1;

endmodule

module xtmux2e(o, d0, d1, s);
   parameter size = 32; // vhdl generic

   output [size-1:0] o;
   input [size-1:0]  d0;
   input [size-1:0]  d1;
   input 	     s;
   
   assign o = (~s) ? d0 : d1;

endmodule

module xtmux3e(o, d0, d1, d2, s);
   parameter size = 32; // vhdl generic

   output [size-1:0] o;
   input [size-1:0]  d0;
   input [size-1:0]  d1;
   input [size-1:0]  d2;
   input [1:0] 	     s;
   reg [size-1:0]    o;

   always @(s or d0 or d1 or d2) begin : xtmux3e
      case (s)		
	2'b00:
	  o = d0;
	2'b01:
	  o = d1;
	2'b10:
	  o = d2;
	default:
`ifdef NOXPROP
	  o = {size{1'b0}};
`else
	  o = {size{1'bx}};
`endif
      endcase // case(sel)
   end // always @ (s or d0 or d1 or d2)

endmodule

module xtmux4e(o, d0, d1, d2, d3, s);
   parameter size = 32; // vhdl generic

   output [size-1:0] o;
   input [size-1:0]  d0;
   input [size-1:0]  d1;
   input [size-1:0]  d2;
   input [size-1:0]  d3;
   input [1:0]	     s;
   reg [size-1:0]    o;
   
   always @(s or d0 or d1 or d2 or d3) begin : xtmux4e
      case (s)	
	2'b00:
	  o = d0;
	2'b01:
	  o = d1;
	2'b10:
	  o = d2;
	2'b11:
	  o = d3;
	default:
`ifdef NOXPROP
	  o = {size{1'b0}};
`else
	  o = {size{1'bx}};
`endif
      endcase // case(sel)
   end // always @ (s or d0 or d1 or d2 or d3) //'

endmodule

module xtnscflop(xtout, xtin, clrb, clk);
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input	     clrb;
   input 	     clk;
   reg [size-1:0]    tmp;
   assign xtout = tmp;
   always @(negedge clk) begin
         tmp <= #1 (!clrb) ? {size{1'b0}} : xtin;
   end // always @ (negedge clk)

endmodule

module xtscflop(xtout, xtin, clrb, clk);	// sync clear ff
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input	     clrb;
   input 	     clk;
   reg [size-1:0]    tmp;
   // synopsys sync_set_reset "clrb"
   // cadence sync_set_reset "clrb"
   assign xtout = tmp;
   always @(posedge clk) begin
         tmp <= #1 (!clrb) ? {size{1'b0}} : xtin;
   end

endmodule

module xtscenflop(xtout, xtin, en, clrb, clk);	// sync clear 
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input 	     en;
   input             clrb;
   input 	     clk;
   reg [size-1:0]    tmp;
   // synopsys sync_set_reset "clrb"
   // cadence sync_set_reset "clrb"
   assign xtout = tmp;
   always @(posedge clk) begin
         tmp <= #1 (!clrb) ? {size{1'b0}} : (en) ? xtin : tmp;
   end

endmodule

module xtssflop(xtout, xtin, setb, xtsval, clk);	// sync set
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input [size-1:0]  xtsval;
   input             setb;
   input 	     clk;
   wire [size-1:0]    tmp_int;

   wire [size-1:0]    tmp = tmp_int ^ xtsval;
   wire [size-1:0]    xtin_int = xtin ^ xtsval;
   xtscflop #(size) scflop_i (tmp_int, xtin_int, setb, clk);
   assign xtout = tmp;
endmodule 

module xtssenflop(xtout, xtin, en, setb, xtsval, clk);	// sync set
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input [size-1:0]  xtsval;
   input 	     en;
   input             setb;
   input 	     clk;
   wire [size-1:0]    tmp_int;

   wire [size-1:0]    tmp = tmp_int ^ xtsval;
   wire [size-1:0]    xtin_int = xtin ^ xtsval;
   xtscenflop #(size) scenflop_i (tmp_int, xtin_int, en, setb, clk);
   assign xtout = tmp;
endmodule

module xtspflop(xtout, xtin, presetb, clk);	// sync preset ff
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input	     presetb;
   input 	     clk;
   reg [size-1:0]    tmp;
   // synopsys sync_set_reset "presetb"
   // cadence sync_set_reset "presetb"
   assign xtout = ~tmp;
   always @(posedge clk) begin
         tmp <= #1 (!presetb) ? {size{1'b0}} : ~xtin;
   end

endmodule // xtspflop

module xtspenflop(xtout, xtin, en, presetb, clk);       // sync preset ff
   parameter size = 32; // vhdl generic

   output [size-1:0] xtout;
   input [size-1:0]  xtin;
   input             en;
   input             presetb;
   input             clk;
   reg [size-1:0]    tmp;

   // synopsys sync_set_reset "presetb"
   // cadence sync_set_reset "presetb"

   assign xtout = ~tmp;
   always @(posedge clk) begin
      tmp <= #1 (!presetb) ? {size{1'b0}} :
                (en)       ? ~xtin :
		             tmp;
   end

endmodule // xtspflop

module xtclock_gate_xor(xtout,xtin1,xtin2);
 output xtout;
 input xtin1,xtin2;
 
 assign xtout = (xtin1 ^ xtin2);

endmodule

module xtclock_gate_nor(xtout,xtin1,xtin2);
 output xtout;
 input xtin1,xtin2;
 
 assign xtout = ~(xtin1 | xtin2);

endmodule

module xtclock_gate_or(xtout,xtclk,xten);
output xtout;
input  xtclk;
input  xten;
    assign xtout = (xten | xtclk);
endmodule

module xtclock_gate_nlatch(xtout,xtin,en);
 output xtout;
 input xtin;
 input en;

 reg xtout;

 always @(en or xtin) begin
  if (~en) begin
   xtout <= #1 xtin;
  end
 end
endmodule // xtclock_gate_nlatch

module xtclock_gate_and(xtout,xtclk,xten);
   output xtout;
   input  xtclk;
   input  xten;


 assign xtout = (xten & xtclk);

endmodule // xtclock_gate_and

module xtgated_clock(xtout, xten, xtclk);
output xtout;
input xten, xtclk;

    // IMPORTANT NOTE: Do not edit this file. You cannot remove clock gating 
    // from the design by changing this primitive. Consult the Tensilica 
    // Technical Support website for further information on the removal 
    // of clock gating.

    wire temp;
    xtclock_gate_nlatch i0(temp, xten, xtclk);
    xtclock_gate_and i1(xtout, xtclk, temp);
endmodule

module xtgated_tmode_clock(xtout, xtmode, xten, xtclk);
output xtout;
input xtmode, xten, xtclk;

    // IMPORTANT NOTE: Do not edit this file. You cannot remove clock gating 
    // from the design by changing this primitive. Consult the Tensilica 
    // Technical Support website for further information on the removal 
    // of clock gating.

    wire en = xtmode | xten;
    xtgated_clock i0(xtout, en, xtclk);
endmodule

module xtclock_buffer (xtclkout, xtclkin);
output xtclkout;
input xtclkin;
    assign xtclkout = xtclkin;
endmodule  // xtclock_buffer

module xtreset_async(xtfreset, xtlreset, xtreset, xtclk);
output xtfreset, xtlreset;
input xtreset, xtclk;
   wire fresetb;
   xtascflop #(1) asyncreset_fresetb_reg(fresetb,1'b1,!xtreset,xtclk);
   assign xtfreset = !fresetb;
   assign xtlreset = !fresetb;
endmodule

module xtreset_tmode_async(xtfreset, xtlreset, xtmode, xtreset, xtclk);
output xtfreset, xtlreset;
input xtmode, xtreset, xtclk;
   wire fresetb;
   xtascflop #(1) asyncreset_fresetb_reg(fresetb,1'b1,!xtreset,xtclk);
   assign xtfreset = xtmode ? xtreset : !fresetb;
   assign xtlreset = !fresetb;
endmodule

module xtreset_async_full(xtfreset, xtlreset, xtreset, xtclk);
output xtfreset, xtlreset;
input xtreset, xtclk;
   wire hresetb, fresetb;
   xtascflop #(1) asyncreset_hresetb_reg(hresetb,1'b1,!xtreset,xtclk);
   xtascflop #(1) asyncreset_fresetb_reg(fresetb,hresetb,!xtreset,xtclk);
   assign xtfreset = !fresetb;
   assign xtlreset = !fresetb;
endmodule

module xtreset_tmode_async_full(xtfreset, xtlreset, xtmode, xtreset, xtclk);
output xtfreset, xtlreset;
input xtmode, xtreset, xtclk;
   wire hresetb, fresetb;
   xtascflop #(1) asyncreset_hresetb_reg(hresetb,1'b1,!xtreset,xtclk);
   xtascflop #(1) asyncreset_fresetb_reg(fresetb,hresetb,!xtreset,xtclk);
   assign xtfreset = xtmode ? xtreset : !fresetb;
   assign xtlreset = !fresetb;
endmodule

module xtcmp(xtout, a, b, s);
   parameter size = 32; // vhdl generic
   output [4:0] xtout;
   input [size-1:0] a, b;
   input s;

`ifdef DW_DIRECT_PRIMITIVES
   wire lt,gt,eq,le,ge,ne;
   DW01_cmp6 #(size) u1 (.A(a),.B(b),.TC(s),.LT(lt),.GT(gt),.EQ(eq),.LE(le),.GE(ge),.NE(ne));
`else
   wire eq = a == b;
   wire ule = a <= b;
   wire aneg = a[size-1];
   wire bpos = ~b[size-1];
   wire sle = (aneg & bpos) | (ule & aneg) | (ule & bpos);
   wire le = s ? sle : ule;
   wire lt = le & ~eq;
   wire gt = ~le;
   wire ge = ~lt;
`endif

   assign xtout = {lt, le, eq, ge, gt};
endmodule
