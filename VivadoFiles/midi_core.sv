module midi_core(
    input  logic clk,
    input  logic reset,
    // slot interface
    input  logic cs,
    input  logic read,
    input  logic write,
    input  logic [4:0] addr,
    input  logic [31:0] wr_data,
    output logic [31:0] rd_data,
    // external ports
    input data_in
   );

    typedef enum logic [1:0] {
        IDLE      = 2'b00,
        READING   = 2'b01,
        ANALYZING = 2'b10,
        ERROR     = 2'b11
    } state_type;

    /*
    state_type current_state, next_state;
    
    logic begin_delay_enable, tick, delay_1600_toggle;
    logic [29:0] data_in_array;
    logic [6:0] data_count;
    logic pressRelease, readingEntered, analyzingEntered, errorFlag, errorFlagZEROS, errorFlagNoLETTER, errorFlagDEFAULT, errorFlagStopBITS;
    logic [7:0] keyData, output_letter;
    logic [31:0] buffer_out, next_buffer_out, history_buff, next_history_buff;
    */
        
    (* mark_debug = "true" *) logic        data_in;   // input, but tagging ensures debug visibility
    (* mark_debug = "true" *) logic [31:0] rd_data;   // already declared as output, but tagging ensures debug visibility

    (* mark_debug = "true" *) state_type   current_state, next_state;
    (* mark_debug = "true" *) logic        begin_delay_enable, delay_1600_toggle, tick;
    (* mark_debug = "true" *) logic [29:0] data_in_array;
    (* mark_debug = "true" *) logic [6:0]  data_count;
    (* mark_debug = "true" *) logic        pressRelease, readingEntered, analyzingEntered;
    (* mark_debug = "true" *) logic        errorFlag, errorFlagZEROS, errorFlagNoLETTER, errorFlagDEFAULT, errorFlagStopBITS;
    (* mark_debug = "true" *) logic [7:0]  keyData, output_letter;
    (* mark_debug = "true" *) logic [31:0] buffer_out, next_buffer_out, history_buff, next_history_buff;
    
    counter_toggle counter_1600_delay_unit(clk, reset, begin_delay_enable, 32'd1600, delay_1600_toggle);
    baud_gen_midi baud_gen_unit(clk, reset, delay_1600_toggle, 12'd3200, tick);
    
    
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            current_state <= IDLE;
            buffer_out <= 32'h00000800;
            history_buff <= 32'h00000800;
            readingEntered <= 0;
            analyzingEntered <= 0;
            errorFlag <= 0;
            errorFlagZEROS <= 0;
            errorFlagNoLETTER <= 0;
            errorFlagDEFAULT <= 0;
            errorFlagStopBITS <= 0;
            pressRelease <= 0;
            output_letter <= 8'b00010000;
        end
        else begin
            current_state <= next_state;
            buffer_out <= next_buffer_out;
            history_buff <= next_history_buff;
        end
    end

    always_comb begin
        if (addr != 3)
            rd_data = buffer_out;   
        else
            rd_data = history_buff;
            
        next_history_buff = data_in;
        
        next_buffer_out[7:0]  = output_letter;
        next_buffer_out[8]   = pressRelease;
        next_buffer_out[9]   = readingEntered;
        next_buffer_out[10]   = analyzingEntered;
        next_buffer_out[11]   = errorFlagZEROS;
        next_buffer_out[12]   = errorFlagNoLETTER;
        next_buffer_out[13]   = errorFlagStopBITS;
        next_buffer_out[14]   = errorFlagDEFAULT;
        next_buffer_out[15]   = errorFlag;
        next_buffer_out[31:16] = 16'b1111111100000000;
        
        if (next_buffer_out[13:11] != 3'b000)
            errorFlag = 1;
            
        case (current_state)
        
            IDLE: begin
                begin_delay_enable = 0;
                data_count = 0;
                data_in_array = 0;
                if (data_in == 0)
                    next_state = READING;
                else
                    next_state = IDLE;
                //next_state = (data_in == 0) ? READING : IDLE;
            end
            
            READING: begin
                begin_delay_enable = 1;     //Enable 1600 count delay
                readingEntered = 1;
                if (tick) begin             //baudGen should be started by the 1600 counter
                    data_in_array[data_count] = data_in;
                    data_count = data_count + 1;
                end
                
                if (data_count >= 30)
                    next_state = ANALYZING;
                else
                    next_state = READING;
                //next_state = (data_count >= 30) ? ANALYZING : READING;
            end
           
            ANALYZING: begin
                begin_delay_enable = 0;
                analyzingEntered = 1;
                if (data_in_array == 32'h00000000) begin
                    errorFlagZEROS = 1;
                    output_letter = 8'b00000010;
                end else begin
                    errorFlagZEROS = 0;
                    pressRelease = data_in_array[5];
                    keyData = data_in_array[18:11];
                    output_letter = keyData;
                end
                    
                if ((data_in_array[0] == 0) && (data_in_array[29] == 1))
                    errorFlagStopBITS = 0;
                else 
                    errorFlagStopBITS = 1;
                
                next_state = IDLE;
                //next_state = IDLE;
            end
            
            default: begin
                errorFlagDEFAULT = 1;
                next_state = IDLE;
            end
        endcase
    end

endmodule 



//    typedef enum logic [6:0] {
//        C0 = 7'd12,
//        D0 = 7'd14,
//        E0 = 7'd16,
//        F0 = 7'd17,
//        G0 = 7'd19,
//        A0 = 7'd21,
//        B0 = 7'd23,
//        C1 = 7'd24,
//        D1 = 7'd26,
//        E1 = 7'd28,
//        F1 = 7'd29,
//        G1 = 7'd31,
//        A1 = 7'd33,
//        B1 = 7'd35,
//        C2 = 7'd36,
//        D2 = 7'd38,
//        E2 = 7'd40,
//        F2 = 7'd41,
//        G2 = 7'd43,
//        A2 = 7'd45,
//        B2 = 7'd47,
//        C3 = 7'd48,
//        D3 = 7'd50,
//        E3 = 7'd52,
//        F3 = 7'd53,
//        G3 = 7'd55,
//        A3 = 7'd57,
//        B3 = 7'd59,
//        C4 = 7'd60,
//        D4 = 7'd62,
//        E4 = 7'd64,
//        F4 = 7'd65,
//        G4 = 7'd67,
//        A4 = 7'd69,
//        B4 = 7'd71,
//        C5 = 7'd72
//    } key_map_type;


