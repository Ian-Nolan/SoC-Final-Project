module counter_toggle(
    input  logic clk,
    input  logic reset,
    input  logic begin_delay_enable,
    input  logic [31:0] upper_bound,
    output logic toggle_out_signal
   );
   
   logic [31:0] count, ncount;

   always_ff @(posedge clk, posedge reset)
      if (reset) begin
         count <= 0;
//         out_signal_buffer <= 0;
//         next_out_signal_buffer <= 0;
      end
      else begin
//         out_signal_buffer <= next_out_signal_buffer;
         count <= ncount;
//         if (begin_delay_enable) begin
//             count <= ncount;
//         end
//         else
//            count <= 0;
      end
   
   always_comb begin
        //toggle_out_signal = out_signal_buffer;
        toggle_out_signal = (count >= upper_bound) ? 1 : 0;
        
        
        if (begin_delay_enable) begin
            if (count >= upper_bound + 3)
                ncount = count;
            else
                ncount = count + 1;
        end
        else
            ncount = 0;
        
//        if (count >= upper_bound + 5) begin
//            ncount = count;
//            //next_out_signal_buffer = 1;
//        end
//        else begin
//            ncount = count + 1;
//            //next_out_signal_buffer = 0;
//        end
    end
    
endmodule


