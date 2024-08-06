#pragma once

#include "tcl/engine/tcl_cmd.hpp"
#include "tcl/engine/tcl_option.hpp"
#include "tcl/cmds/utils.hpp"

#include "misc/anchor.hpp"

#include "layer_arch/arch_manager.hpp"
#include "layer_logic/logic_manager.hpp"
#include "layer_netlist/netlist_manager.hpp"

#include "layer_netlist/api/ieda/pass/init.hpp"

namespace lf
{

namespace tcl
{
class CmdLfStart : public TclCmd
{
public:
  explicit CmdLfStart( const char* cmd_name )
      : TclCmd( cmd_name )
  {
    // set the description
    std::string description = "";
    this->set_description( description );
    // set the domain
    std::string domain = "flow";
    this->set_domain( domain );
  }

  ~CmdLfStart() override = default;

  unsigned check() override
  {
    return 1;
  }

  unsigned exec() override
  {
    if ( !check() )
      return 0;

    lfAmINST->start();
    lfLmINST->start();
    lfNmINST->start();

    return 1;
  }

}; // class CmdLfStart

class CmdLfInit : public TclCmd
{
public:
  explicit CmdLfInit( const char* cmd_name )
      : TclCmd( cmd_name )
  {
    // set the description
    std::string description = "";
    this->set_description( description );
    std::string domain = "flow";
    this->set_domain( domain );
    // set the options
    std::vector<lfCmdOption> options = {
        { "-file", "ieda", "string", "" } };

    setOptions( this, options );
  }

  ~CmdLfInit() override = default;

  unsigned check() override
  {
    std::vector<std::string> essential = { "-file" };
    return checkEssentialOptions( this, essential );
  }

  unsigned exec() override
  {
    if ( !check() )
      return 0;

    std::map<std::string, std::string> strOptionsValue;
    std::map<std::string, bool> boolOptionsValue;
    std::map<std::string, int> intOptionsValue;
    std::map<std::string, double> doubleOptionsValue;
    std::map<std::string, std::vector<std::string>> strvecOptionsValue;
    std::map<std::string, std::vector<int>> intvecOptionsValue;
    std::map<std::string, std::vector<double>> doublevecOptionsValue;

    std::vector<std::string> strOptions = { "-file" };
    std::vector<std::string> boolOptions = {};
    std::vector<std::string> intOptions = {};
    std::vector<std::string> doubleOptions = {};
    std::vector<std::string> strvecOptions = {};
    std::vector<std::string> intvecOptions = {};
    std::vector<std::string> doublevecOptions = {};

    extractOptions( this, strOptions, boolOptions, intOptions, doubleOptions, strvecOptions, intvecOptions, doublevecOptions,
                    strOptionsValue, boolOptionsValue, intOptionsValue, doubleOptionsValue, strvecOptionsValue, intvecOptionsValue, doublevecOptionsValue );

    auto anchor_domain = lfAnchorINST->get_anchor_domain();

    switch ( anchor_domain )
    {
    case lf::misc::E_LF_ANCHOR_DOMAIN::E_LF_ANCHOR_DOMAIN_NETLIST_IEDA:
      lf::netlist::ieda::run_init( strOptionsValue["-file"] );
      break;
    default:
      std::cerr << "Unsupported anchor domain, please use anchor to set the anchor!" << std::endl;
      return 0;
    }
    return 1;
  }
}; // class CmdLfInit

class CmdLfStop : public TclCmd
{
public:
  explicit CmdLfStop( const char* cmd_name )
      : TclCmd( cmd_name )
  {
    // set the description
    std::string description = "";
    this->set_description( description );
    // set the domain
    std::string domain = "flow";
    this->set_domain( domain );
  }

  ~CmdLfStop() override = default;

  unsigned check() override
  {
    return 1;
  }

  unsigned exec() override
  {
    if ( !check() )
      return 0;

    lfAmINST->stop();
    lfLmINST->stop();
    lfNmINST->stop();

    return 1;
  }

}; // class CmdLfStop

class CmdLfAnchor : public TclCmd
{
public:
  explicit CmdLfAnchor( const char* cmd_name )
      : TclCmd( cmd_name )
  {
    // set the description
    std::string description = "";
    this->set_description( description );
    // set the domain
    std::string domain = "flow";
    this->set_domain( domain );

    std::vector<std::pair<std::string, std::string>> str_common = {
        std::make_pair( "-set", "[flow], set the anchor for following passes, value at {yosys, abc, lsils, ieda}" ) };
    for ( auto [args, desc] : str_common )
    {
      auto* option = new TclStringOption( args.c_str(), 1, nullptr );
      option->set_description( desc );
      addOption( option );
    }
  }

  ~CmdLfAnchor() override = default;

  unsigned check() override
  {
    std::vector<std::string> essential = {
        "-set" };
    for ( auto& ess : essential )
    {
      if ( !getOptionOrArg( ess.c_str() ) )
      {
        std::cerr << "Essential option " << ess << " is not set!" << std::endl;
        assert( false );
        return 0;
      }
    }
    return 1;
  }

  unsigned exec() override
  {
    if ( !check() )
      return 0;

    lfAmINST->start();
    lfLmINST->start();
    lfNmINST->start();

    std::string anchor = "";
    TclOption* option_anchor = getOptionOrArg( "-set" );
    if ( option_anchor->is_set_val() )
      anchor = option_anchor->getStringVal();

    if ( anchor == "yosys" )
    {
      lfAnchorINST->set_anchor( lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_ARCH_YOSYS_INIT );
    }
    else if ( anchor == "abc" )
    {
      lfAnchorINST->set_anchor( lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_ABC_INIT );
    }
    else if ( anchor == "lsils" )
    {
      lfAnchorINST->set_anchor( lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_LOGIC_LSILS_INIT );
    }
    else if ( anchor == "ieda" )
    {
      lfAnchorINST->set_anchor( lf::misc::E_LF_ANCHOR::E_LF_ANCHOR_NETLIST_IEDA_INIT );
    }
    else
    {
      std::cerr << "Unknown anchor: " << std::endl;
      return 0;
    }

    return 1;
  }

}; // class CmdLfAnchor

} // namespace tcl

} // namespace lf