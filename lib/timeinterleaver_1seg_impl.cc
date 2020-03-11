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

      const int timeinterleaver_1seg_impl::d_data_carriers_mode1 = 96; 
      const int timeinterleaver_1seg_impl::d_total_segments = 13; 

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
     /*         gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))),
              gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))))*/
                    gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))),
                    gr::io_signature::make(1, 1, sizeof(gr_complex)*d_data_carriers_mode1*((int)pow(2.0,mode-1))))
    {
            d_mode = mode; 
            d_I_A = length_A; 
            d_nsegments_A = segments_A;


            d_carriers_per_segment = d_data_carriers_mode1*((int)pow(2.0,mode-1)); 
            d_noutput = d_carriers_per_segment; 
            int mi = 0;

            int sync_delay = 0;

            for (int segment=0; segment<d_nsegments_A; segment++)
            {
                sync_delay = 204 - ((95*d_I_A) % 204);
                for (int carrier = 0; carrier<d_carriers_per_segment; carrier++)
                {
                    mi = (5*carrier) % d_data_carriers_mode1; 
                    //d_shift.push_back(new std::deque<gr_complex>(d_I*(d_data_carriers_mode1-1-mi),0)); 
                    d_shift.push_back(new boost::circular_buffer<gr_complex>(d_I_A*mi + 1 + sync_delay, 0)); 
                }
            }


    }

    /*
     * Our virtual destructor.
     */
    timeinterleaver_1seg_impl::~timeinterleaver_1seg_impl()
    {
                for (unsigned int i=0; i<d_shift.size();i++){
                delete d_shift.back();
                d_shift.pop_back();
            }
    }

    int
    timeinterleaver_1seg_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for (int i=0; i<noutput_items; i++)
                {
                    for (int carrier=0; carrier<d_carriers_per_segment; carrier++)
                    {
                        // a simple FIFO queue performs the interleaving. 
                        // The "difficult" part is setting the correct sizes 
                        // for each queue. 
                        d_shift[carrier]->push_back(in[i*d_noutput + carrier]);
                        out[i*d_carriers_per_segment + carrier] = d_shift[carrier]->front();
                        //d_shift[carrier]->pop_front(); 
                    }
                    /*for (int carrier=d_carriers_per_segment; carrier<d_noutput; carrier++)
                    {
                        // a test: delete this part after working
          
                        out[i*d_noutput + carrier] = 0;
                        //d_shift[carrier]->pop_front(); 
                    }*/
                }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace isdbt */
} /* namespace gr */

