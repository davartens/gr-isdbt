# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND isdbt_sources
    ofdm_sym_acquisition_impl.cc
    sync_and_channel_estimation_impl.cc
    subset_of_carriers_impl.cc
    frequency_deinterleaver_impl.cc
    time_deinterleaver_impl.cc
    symbol_demapper_impl.cc
    tmcc_decoder_impl.cc
    bit_deinterleaver_impl.cc
    d_viterbi.c
    d_metrics.c
    d_tab.c
    energy_descrambler_impl.cc
    byte_deinterleaver_impl.cc
    #bit_deinterleaver_streamed_impl.cc
    ofdm_sym_acquisition_dvbt_impl.cc
    reed_solomon_dec_isdbt_impl.cc 
    #reed_solomon.cc
    frequency_deinterleaver_1seg_impl.cc
    time_deinterleaver_1seg_impl.cc
    symbol_demapper_1seg_impl.cc
    viterbi_decoder_impl.cc
    ofdm_synchronization_impl.cc
    ofdm_synchronization_1seg_impl.cc
    tmcc_decoder_1seg_impl.cc
    pilot_signals_impl.cc
    frequency_interleaver_impl.cc
    time_interleaver_impl.cc
    hierarchical_combinator_impl.cc
    carrier_modulation_impl.cc
    byte_interleaver_impl.cc
    energy_dispersal_impl.cc
    tmcc_encoder_impl.cc
    hierarchical_divisor_impl.cc
    tsp_resize_impl.cc )

set(isdbt_sources "${isdbt_sources}" PARENT_SCOPE)
if(NOT isdbt_sources)
    MESSAGE(STATUS "No C++ sources... skipping lib/")
    return()
endif(NOT isdbt_sources)

add_library(gnuradio-isdbt SHARED ${isdbt_sources})
target_link_libraries(gnuradio-isdbt ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-isdbt PROPERTIES DEFINE_SYMBOL "gnuradio_isdbt_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-isdbt PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
        )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-isdbt
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
    )

#######################################################################
## Workaround for issues with the change of VOLK API
## See https://github.com/gnuradio/volk/issues/84
## and https://github.com/git-artes/gr-isdbt/issues/18
#######################################################################

if(NOT VOLK_VERSION)
    set(VOLK_VERSION "1.2.2")
endif(NOT VOLK_VERSION)

if(VOLK_VERSION VERSION_EQUAL 1.2.2)
    find_file(VOLK_GREATER_THAN_122
        NAMES volk/volk_32f_index_max_32u.h
        HINTS ${CMAKE_INSTALL_PREFIX}/include
        PATHS /usr/local/include
        /usr/include
        ${CMAKE_INSTALL_PREFIX}/include
        )
    if(VOLK_GREATER_THAN_122)
        set(VOLK_VERSION "1.2.3+")
    endif(VOLK_GREATER_THAN_122)
endif(VOLK_VERSION VERSION_EQUAL 1.2.2)

if(VOLK_VERSION VERSION_GREATER 1.2.2)
    add_definitions(-DVOLK_GT_122=1)
endif(VOLK_VERSION VERSION_GREATER 1.2.2)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})
list(APPEND test_isdbt_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_isdbt.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_isdbt.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_tmcc_encoder.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_energy_dispersal.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_byte_interleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_carrier_modulation.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_hierarchical_combinator.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_time_interleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_frequency_interleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_pilot_signals.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_tmcc_decoder_1seg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ofdm_synchronization_1seg.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ofdm_synchronization.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_reed_solomon_dec_isdbt.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_byte_deinterleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_energy_descrambler.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_bit_deinterleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_symbol_demapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_time_deinterleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_frequency_deinterleaver.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_ofdm_sym_acquisition.cc
)

add_executable(test-isdbt ${test_isdbt_sources})

target_link_libraries(
  test-isdbt
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-isdbt
)

GR_ADD_TEST(test_isdbt test-isdbt)
