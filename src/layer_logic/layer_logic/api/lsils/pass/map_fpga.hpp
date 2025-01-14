#pragma once

#include "layer_logic/logic_manager.hpp"

#include "mockturtle/networks/klut.hpp"
#include "mockturtle/networks/sequential.hpp"

#include "mockturtle/views/mapping_view.hpp"
#include "mockturtle/algorithms/lut_mapping.hpp"
#include "mockturtle/algorithms/collapse_mapped.hpp"

namespace lf
{

namespace logic
{

namespace lsils
{

/**
 * @brief Technology Mapping
 * @example
 *  rewrite [options]
 *  options: [KCDA] [v]
 * @note
 */
void map_fpga( int K_feasible_cut = -1, int Cut_limit = -1, int DelayIter = -1, int AreaIter = -1,
               bool is_verbose = false )
{
  printf( "map fpga\n" );
  mockturtle::lut_mapping_params ps;
  if ( K_feasible_cut > 0 )
    ps.cut_enumeration_ps.cut_size = K_feasible_cut;
  if ( Cut_limit > 0 )
    ps.cut_enumeration_ps.cut_limit = Cut_limit;
  if ( DelayIter > 0 )
    ps.rounds = DelayIter;
  if ( AreaIter > 0 )
    ps.rounds_ela = AreaIter;
  if ( is_verbose )
    ps.verbose = true;

  auto ntktype = lfLntINST->get_ntktype_curr();
  assert( ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AIG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_OIG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AOG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XAG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XOG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XMG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_MIG ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_PRIMARY ||
          ntktype == lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_GTG );
  if ( ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AIG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_OIG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AOG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XAG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XOG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XMG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_MIG &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_PRIMARY &&
       ntktype != lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_GTG )
  {
    std::cerr << "[ERROR] write_bench: wrong ntk type!" << std::endl;
    return;
  }

  klut_seq_network netlist_fpga;
  switch ( ntktype )
  {
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AIG:
  {
    lf::logic::lsils::aig_seq_network ntk = lfLmINST->current<lf::logic::lsils::aig_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::aig_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::aig_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_OIG:
  {
    lf::logic::lsils::oig_seq_network ntk = lfLmINST->current<lf::logic::lsils::oig_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::oig_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::oig_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_AOG:
  {
    lf::logic::lsils::aog_seq_network ntk = lfLmINST->current<lf::logic::lsils::aog_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::aog_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::aog_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XAG:
  {
    lf::logic::lsils::xag_seq_network ntk = lfLmINST->current<lf::logic::lsils::xag_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::xag_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::xag_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XOG:
  {
    lf::logic::lsils::xog_seq_network ntk = lfLmINST->current<lf::logic::lsils::xog_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::xog_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::xog_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_XMG:
  {
    lf::logic::lsils::xmg_seq_network ntk = lfLmINST->current<lf::logic::lsils::xmg_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::xmg_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::xmg_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_MIG:
  {
    lf::logic::lsils::mig_seq_network ntk = lfLmINST->current<lf::logic::lsils::mig_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::mig_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::mig_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_PRIMARY:
  {
    lf::logic::lsils::primary_seq_network ntk = lfLmINST->current<lf::logic::lsils::primary_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::primary_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::primary_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped );
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  case lf::misc::E_LF_NTK_TYPE::E_LF_NTK_TYPE_LSILS_STRASH_GTG:
  {
    lf::logic::lsils::gtg_seq_network ntk = lfLmINST->current<lf::logic::lsils::gtg_seq_network>();
    mockturtle::mapping_view<lf::logic::lsils::gtg_seq_network, true> ntk_mapped{ ntk };
    mockturtle::lut_mapping<mockturtle::mapping_view<lf::logic::lsils::gtg_seq_network, true>, true>( ntk_mapped, ps );
    mockturtle::collapse_mapped_network<klut_seq_network>( netlist_fpga, ntk_mapped ); // TODO: failed at function compute by window simulate
    lfLmINST->set_current<klut_seq_network>( netlist_fpga );
    break;
  }
  default:
  {
    std::cerr << "unsupport network type!\n";
    assert( false );
    break;
  }
  }
}

} // end namespace lsils

} // end namespace logic

} // end namespace lf
