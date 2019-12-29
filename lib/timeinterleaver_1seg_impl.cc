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
#include "timeinterleaver_1seg_impl.h"

namespace gr {
  namespace isdbt {

        const int time_interleaver_impl::d_data_carriers_mode1 = 96; 
        const int time_interleaver_impl::d_total_segments = 13; 

    timeinterleaver_1seg::sptr
    timeinterleaver_1seg::make(int mode, int segments_A, int length_A)
    {
      return gnuradio::get_initial_sptr
        (new timeinterleaver_1seg_impl(mode, segments_A, length_A));
    }

    /*
     * The private constructor
     */
    timeinterleaver_1seg_impl::timeinterleaver_1seg_impl(int mode, int segments_A, int length_A)
      : gr::sync_block("timeinterleaver_1seg",
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))))
    {}

    /*
     * Our virtual destructor.
     */
    timeinterleaver_1seg_impl::~timeinterleaver_1seg_impl()
    {
    }

    int
    timeinterleaver_1seg_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const <+ITYPE+> *in = (const <+ITYPE+> *) input_items[0];
      <+OTYPE+> *out = (<+OTYPE+> *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

