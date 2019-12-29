/* -*- c++ -*- */
/*  
 * Copyright 2015, 2016, 2017, 2018
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
#include "padding_impl.h"

namespace gr {
  namespace isdbt {

    int total_portadoras = 8192;
    int portadoras_fft = 512;
    int portadoras_segmento = 432;
    int zeros_on_left = 40;
    int zeros_on_right = 40;
    int comienzo_one_seg = 3880; // 2592 + 1288 = 3880 -- portadoras_de_6_segmentos + zero_padding_fft_8192 = comienzo_one_seg 
    int fin_one_seg = 4311; // 3024+1287	

    padding::sptr
    padding::make()
    {
      return gnuradio::get_initial_sptr
        (new padding_impl());
    }

    /*
     * The private constructor
     */
    padding_impl::padding_impl()
      : gr::sync_block("padding",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*total_portadoras),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*portadoras_fft))
    {}

    /*
     * Our virtual destructor.
     */
    padding_impl::~padding_impl()
    {
    }

    int
    padding_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];
	
	int carrier;
      // Do <+signal processing+>
	for (int i = 0; i < noutput_items; i++){
		
		for (carrier = 0; carrier < zeros_on_left; carrier++) {
			out[i*portadoras_fft + carrier] = 0;
                    }
		
		/*for (carrier = 0; carrier < portadoras_segmento; carrier++) {
			out[i*portadoras_fft + zeros_on_left + carrier] = in[i*total_portadoras + comienzo_one_seg + carrier]; 
                    }*/

		memcpy(out + i*portadoras_fft + zeros_on_left, comienzo_one_seg + in + i*total_portadoras, sizeof(gr_complex)*portadoras_segmento);
			

		for (carrier = 0; carrier < zeros_on_right; carrier++) {
			out[i*portadoras_fft + zeros_on_left + portadoras_segmento + carrier] = 0;
                    }
		
		}

	

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

