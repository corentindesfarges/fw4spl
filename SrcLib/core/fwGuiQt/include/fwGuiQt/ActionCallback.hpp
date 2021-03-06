/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_ACTIONCALLBACK_HPP__
#define __FWGUIQT_ACTIONCALLBACK_HPP__


#include <QObject>

#include <fwGui/ActionCallbackBase.hpp>

#include "fwGuiQt/config.hpp"


namespace fwGuiQt
{

/**
 * @brief   Defines the menu item callback.
 * @class   ActionCallback
 *
 * @date    2009-2010.
 *
 */
class FWGUIQT_CLASS_API ActionCallback :  public QObject,
                                          public ::fwGui::ActionCallbackBase
{

Q_OBJECT

public:

    fwCoreClassDefinitionsWithFactoryMacro( (ActionCallback)(::fwGui::ActionCallbackBase),
                                            (()),
                                            ::fwGui::factory::New< ActionCallback > );

    FWGUIQT_API ActionCallback(::fwGui::GuiBaseObject::Key key);

    FWGUIQT_API virtual ~ActionCallback();

    /**
     * @brief  Qt slot.
     */
public Q_SLOTS:
    void executeQt(bool checked = false);
    void checkQt(bool checked);

};

} // namespace fwGuiQt

#endif /*__FWGUIQT_ACTIONCALLBACK_HPP__*/


