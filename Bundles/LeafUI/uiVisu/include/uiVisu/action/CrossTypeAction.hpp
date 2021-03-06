/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __UIVISU_ACTION_CROSSTYPEACTION_HPP__
#define __UIVISU_ACTION_CROSSTYPEACTION_HPP__

#include "uiVisu/config.hpp"

#include <fwCom/Signal.hpp>
#include <fwCom/Signals.hpp>

#include <fwGui/IActionSrv.hpp>

#include <fwTools/Failed.hpp>

#include <map>
#include <string>

namespace uiVisu
{
namespace action
{

/**
 * @brief   This action allows change the cross type
 * @class   CrossTypeAction
 */
class UIVISU_CLASS_API CrossTypeAction : public ::fwGui::IActionSrv
{

public:

    fwCoreServiceClassDefinitionsMacro ( (CrossTypeAction)(::fwGui::IActionSrv) );

    /// Constructor. Do nothing.
    UIVISU_API CrossTypeAction() throw();

    /// Destructor. Do nothing.
    UIVISU_API virtual ~CrossTypeAction() throw();

protected:

    typedef ::fwRuntime::ConfigurationElement::sptr Configuration;

    /// Starts action
    virtual void starting() throw(::fwTools::Failed);

    /// Stops action
    virtual void stopping() throw(::fwTools::Failed);

    /// Notifies message to change cross type
    void updating() throw(::fwTools::Failed);

    /// Does nothing
    void swapping() throw(::fwTools::Failed);

    /**
     * @brief Configure action.
     * @verbatim
       <service type="::fwGui::IActionSrv" impl="::uiData::action::CrossTypeAction" autoConnect="no">
        <crossType>hide</crossType>
       </service>
       @endverbatim
     * - \b crossType : must be full, half or hide
     */
    void configuring() throw(fwTools::Failed);

    virtual void info( std::ostream &_sstream );

private:

    static std::map< std::string, float > m_scaleConversion;
    std::string m_crossType;

    typedef ::fwCom::Signal< void (double) > CrossTypeModifiedSignalType;
    CrossTypeModifiedSignalType::sptr m_sigCrossTypeModified; ///< Signal emitted when cross type is modified
};

} // action
} // uiVisu

#endif /*__UIVISU_ACTION_CROSSTYPEACTION_HPP__*/


