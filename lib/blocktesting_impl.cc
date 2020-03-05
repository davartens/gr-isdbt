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
#include "blocktesting_impl.h"

namespace gr {
  namespace isdbt {

    blocktesting::sptr
    blocktesting::make()
    {
      return gnuradio::get_initial_sptr
        (new blocktesting_impl());
    }

    /*
     * The private constructor
     */
    blocktesting_impl::blocktesting_impl()
      : gr::sync_block("blocktesting",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*512),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*8192))
    {}

    /*
     * Our virtual destructor.
     */
    blocktesting_impl::~blocktesting_impl()
    {
    }

    int
    blocktesting_impl::work(int noutput_items,
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

		for (carrier = 0; carrier < 432; carrier++) {
			out[i*8192 + carrier + 1288 + 2592] = in[i*512 + carrier + 40];
                    }

    
		for (carrier = 1288 + 2592 + 432; carrier < 8192; carrier++) {
			out[i*8192 + carrier] = 0;
                    }
		
		}
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

