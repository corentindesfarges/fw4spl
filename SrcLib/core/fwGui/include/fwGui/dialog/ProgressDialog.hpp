/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUI_DIALOG_PROGRESSDIALOG_HPP__
#define __FWGUI_DIALOG_PROGRESSDIALOG_HPP__

#include <fwCore/base.hpp>

#include "fwGui/config.hpp"
#include "fwGui/dialog/IProgressDialog.hpp"

namespace fwGui
{
namespace dialog
{
/**
 * @brief   Defines the generic progress dialog for IHM.
 * Use the Delegate design pattern. The specific implementation selection is ensured by fwGuiRegisterMacro.
 * The specific implementation are in fwGuiWX and fwGuiQT libraries
 * @class   ProgressDialog
 *
 */
class FWGUI_CLASS_API ProgressDialog : public IProgressDialog
{

public:

    fwCoreClassDefinitionsWithFactoryMacro( (ProgressDialog)(::fwGui::dialog::IProgressDialog), (()),
                                            new ProgressDialog );

    /// will instanciate the concrete implementation
    FWGUI_API ProgressDialog( const std::string &title = std::string(),  const std::string &msg = std::string() );
    FWGUI_API ~ProgressDialog();

    ///set the title for the dialog
    FWGUI_API void setTitle(const std::string &title);

    ///set the message for the dialog
    FWGUI_API void setMessage(const std::string &message);

    /// action called by ::fwTools::ProgressAdviser
    FWGUI_API void operator()(float percent,std::string msg);


    FWGUI_API void setCancelCallback(CancelCallbackType callback);

    FWGUI_API void hideCancelButton();

protected:

    FWGUI_API void cancelPressed();

    ::fwGui::dialog::IProgressDialog::sptr m_implementation;

};

} //namespace dialog
} // namespace fwGui

#endif /*__FWGUI_DIALOG_PROGRESSDIALOG_HPP__*/


