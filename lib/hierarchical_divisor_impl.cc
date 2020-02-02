/* -*- c++ -*- */
/*  
 * Copyright 2015, 2016, 2017
 *   Federico "Larroca" La Rocca <flarroca@fing.edu.uy>
 *   Pablo Belzarena 
 *   Gabriel Gomez Sena 
 *   Pablo Flores Guridi 
 *   Victor Gonzalez Barbone
 * 
 *   Instituto de Ingenieria Electrica, Facultad de Ingenieria,
 *   Universidad de la Republica, Uruguay.
 *  
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *  
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "hierarchical_divisor_impl.h"

namespace gr {
  namespace isdbt {

    int layer_a_tsp;
    int layer_b_tsp;
    int layer_c_tsp;
    int d_tsp_per_frame;
    int d_segments_A;
    int d_segments_B;
    int d_segments_C;
    int tsp_len = 204;
    int d_mode;
    float d_cp;

    hierarchical_divisor::sptr
    hierarchical_divisor::make(int mode, float cp, int segments_A, int mod_scheme_A, float conv_code_A, int segments_B, int mod_scheme_B, float conv_code_B, int segments_C, int mod_scheme_C, float conv_code_C)
    {
      d_mode = mode;
      d_cp = cp;
      d_tsp_per_frame = (int) round((1+d_cp)*pow(2, 9 + mode));

      d_segments_A = segments_A;
      // in order to avoid signatures sizes 0 (throw error), we set the size as 1 but is not used
      d_segments_B = (segments_B > 0)?segments_B:1;
      d_segments_C = (segments_C > 0)?segments_C:1;


      layer_a_tsp = (int)round((d_segments_A*tsp_len*mod_scheme_A*conv_code_A*96*pow(2, d_mode - 1))/(8*tsp_len));
      layer_b_tsp = (int)round((d_segments_B*tsp_len*mod_scheme_B*conv_code_B*96*pow(2, d_mode - 1))/(8*tsp_len));//
      layer_c_tsp = (int)round((d_segments_C*tsp_len*mod_scheme_C*conv_code_C*96*pow(2, d_mode - 1))/(8*tsp_len));


      return gnuradio::get_initial_sptr
        (new hierarchical_divisor_impl(mode, cp, segments_A, mod_scheme_A, conv_code_A, segments_B, mod_scheme_B, conv_code_B, segments_C, mod_scheme_C, conv_code_C));
    }

    /*
     * The private constructor
     */
    hierarchical_divisor_impl::hierarchical_divisor_impl(int mode, float cp, int segments_A, int mod_scheme_A, float conv_code_A, int segments_B, int mod_scheme_B, float conv_code_B, int segments_C, int mod_scheme_C, float conv_code_C)
      : gr::block("hierarchical_divisor",
              gr::io_signature::make(1, 1, d_tsp_per_frame*204*sizeof(unsigned char)),
              gr::io_signature::make3(1, 3, layer_a_tsp*204*sizeof(unsigned char),
                                            layer_b_tsp*204*sizeof(unsigned char),
                                            layer_c_tsp*204*sizeof(unsigned char))
                                      )
    {}

    /*
     * Our virtual destructor.
     */
    hierarchical_divisor_impl::~hierarchical_divisor_impl()
    {
    }

    void
    hierarchical_divisor_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      ninput_items_required[0] = noutput_items;
    }

    int
    hierarchical_divisor_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out0 = (unsigned char *) output_items[0];
      unsigned char *out1 = (unsigned char *) output_items[1];
      unsigned char *out2 = (unsigned char *) output_items[2];

      unsigned char tsp_info_byte; //buffer to read layer information
      int layer_flag = 0;
      int tsp_a = 0;
      int tsp_b = 0;
      int tsp_c = 0;
      int def = 0;
	
      bool out_B_connected = output_items.size()>=2;
      bool out_C_connected = output_items.size()>=3;	
        
      for (int output = 0; output < noutput_items; output++)
      {
        //printf("NUEVO OUTPUT ITEM\n");

	for (int k=0; k<d_tsp_per_frame ; k++)
        {
          //Read first TSP from input
          tsp_info_byte = in[output*d_tsp_per_frame*tsp_len + k*tsp_len + 188 + 1];
          layer_flag = tsp_info_byte >>= 4;
          

          if (layer_flag == 1){
            //Copy TSP to output A
            memcpy(out0+ output*layer_a_tsp*tsp_len + tsp_len*tsp_a, in + k*tsp_len + output*d_tsp_per_frame*tsp_len, tsp_len);
            tsp_a++;
           }
           
           if ((out_B_connected) && (layer_flag==2)){
              //Copy TSP to output B
              memcpy(out1+ output*layer_b_tsp*tsp_len + tsp_len*tsp_b, in + k*tsp_len + output*d_tsp_per_frame*tsp_len, tsp_len);
              tsp_b++;
            }
           
            if ((out_C_connected)&&(layer_flag==3)){
              //Copy TSP to output C
              memcpy(out2+ output*layer_c_tsp*tsp_len + tsp_len*tsp_c, in + k*tsp_len + output*d_tsp_per_frame*tsp_len, tsp_len);
              tsp_c++;
            }
	    if (layer_flag==0){
              //printf("Layer X, total: %i \n", def);
              def++;
             }
          
	

          //Goto test new TSP
        }
        tsp_a = 0;
        tsp_b = 0;
        tsp_c = 0;
        def = 0;
      }
      // Tell runtime system how many input items we consumed on
      // each input stream.
      this->consume(0, noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

