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
#include "pilotstesting_impl.h"

namespace gr {
  namespace isdbt {

    pilotstesting::sptr
    pilotstesting::make()
    {
      return gnuradio::get_initial_sptr
        (new pilotstesting_impl());
    }

    /*
     * The private constructor
     */
    pilotstesting_impl::pilotstesting_impl()
      : gr::sync_block("pilotstesting",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*432*2),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*8192))
    {}

    /*
     * Our virtual destructor.
     */
    pilotstesting_impl::~pilotstesting_impl()
    {
    }

    int
    pilotstesting_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)

    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex  *out = (gr_complex *) output_items[0];

    int carrier;
      // Do <+signal processing+>
        for (int i = 0; i < noutput_items; i++){
		
		for (carrier = 0; carrier < 1288 + 2592; carrier++) {
			out[i*8192 + carrier] = 0;
                    }

		for (carrier = 0; carrier < 432*2; carrier++) {
			out[i*8192 + carrier + 1288 + 2592] = in[i*432*2 + carrier];
                    }

    
		for (carrier = 1288 + 2592 + 432*2; carrier < 8192; carrier++) {
			out[i*8192 + carrier] = 0;
                    }
		
		}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

