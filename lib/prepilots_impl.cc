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
#include "prepilots_impl.h"

namespace gr {
  namespace isdbt {

    prepilots::sptr
    prepilots::make()
    {
      return gnuradio::get_initial_sptr
        (new prepilots_impl());
    }

    /*
     * The private constructor
     */
    prepilots_impl::prepilots_impl()
      : gr::sync_block("prepilots",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*4992),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*384))
    {}

    /*
     * Our virtual destructor.
     */
    prepilots_impl::~prepilots_impl()
    {
    }

    int
    prepilots_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int i=0; i< noutput_items; i++){
        for (int carrier=0; carrier<384; carrier++)
          {
            out[i*384 + carrier] = in[i*4992 + carrier];
          }



      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

