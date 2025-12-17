module baud_gen_midi
   (
    input  logic clk, reset,
    input  logic enable,
    input  logic [11:0] upper_bound,
    output logic tick
   );

   logic [11:0] count;
   logic [11:0] n_count;

   always_ff @(posedge clk, posedge reset)
      if (reset)
         count <= 0;
      else begin
         if (enable)
            count <= n_count;
         else
            count <= 0;
      end

   assign n_count = (count==(upper_bound-1)) ? 0 : count + 1;
   assign tick = (count==1);
endmodule