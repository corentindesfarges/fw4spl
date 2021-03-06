/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGuiQt/dialog/LocationDialog.hpp"

#include <fwData/location/Folder.hpp>
#include <fwData/location/MultiFiles.hpp>
#include <fwData/location/SingleFile.hpp>

#include <fwGui/dialog/ILocationDialog.hpp>
#include <fwGui/registry/macros.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/tokenizer.hpp>
#include <functional>
#include <QApplication>

#include <QFileDialog>
#include <QString>


fwGuiRegisterMacro( ::fwGuiQt::dialog::LocationDialog, ::fwGui::dialog::ILocationDialog::REGISTRY_KEY );


namespace fwGuiQt
{
namespace dialog
{

//------------------------------------------------------------------------------

LocationDialog::LocationDialog(::fwGui::GuiBaseObject::Key key) :
    m_style(::fwGui::dialog::ILocationDialog::NONE),
    m_type(::fwGui::dialog::ILocationDialog::SINGLE_FILE)
{
}

//------------------------------------------------------------------------------

::fwData::location::ILocation::sptr LocationDialog::show()
{
    QWidget *parent                             = qApp->activeWindow();
    QString caption                             = QString::fromStdString(this->getTitle());
    const ::boost::filesystem::path defaultPath = this->getDefaultLocation();
    QString path                                = QString::fromStdString(defaultPath.string());
    QString filter                              = this->fileFilters();
    ::fwData::location::ILocation::sptr location;

    if (m_type == ::fwGui::dialog::ILocationDialog::MULTI_FILES)
    {
        SLM_ASSERT("MULTI_FILES type must have a READ style", m_style & ::fwGui::dialog::ILocationDialog::READ);

        QStringList files = QFileDialog::getOpenFileNames( parent, caption, path, filter);
        if(!files.isEmpty())
        {
            ::fwData::location::MultiFiles::sptr multifiles = ::fwData::location::MultiFiles::New();
            std::vector< ::boost::filesystem::path > paths;
            for (QString filename : files)
            {
                ::boost::filesystem::path bpath( filename.toStdString() );
                paths.push_back(bpath);
            }
            multifiles->setPaths(paths);
            location = multifiles;
        }
    }
    else if (m_type == ::fwGui::dialog::ILocationDialog::SINGLE_FILE)
    {
        QString fileName;
        if ( (m_style & ::fwGui::dialog::ILocationDialog::READ) ||
             (m_style & ::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST) )
        {
            fileName = QFileDialog::getOpenFileName(parent, caption, path, filter);

        }
        else if ( m_style & ::fwGui::dialog::ILocationDialog::WRITE )
        {
            fileName = QFileDialog::getSaveFileName(parent, caption,  path,  filter);

        }
        if(!fileName.isNull())
        {
            ::boost::filesystem::path bpath( fileName.toStdString());
            location = ::fwData::location::SingleFile::New(bpath);
        }
    }
    else if (m_type == ::fwGui::dialog::ILocationDialog::FOLDER)
    {
        QString dir = QFileDialog::getExistingDirectory(parent, caption, path,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(!dir.isNull())
        {
            ::boost::filesystem::path bpath( dir.toStdString()  );
            location = ::fwData::location::Folder::New(bpath);
        }
    }
    return location;
}

//------------------------------------------------------------------------------

void LocationDialog::setType( ::fwGui::dialog::ILocationDialog::Types type )
{
    m_type = type;
}

//------------------------------------------------------------------------------

::fwGui::dialog::ILocationDialog&  LocationDialog::setOption( ::fwGui::dialog::ILocationDialog::Options option)
{
    if ( option == ::fwGui::dialog::ILocationDialog::WRITE )
    {
        m_style = (::fwGui::dialog::ILocationDialog::Options) (m_style & ~::fwGui::dialog::ILocationDialog::READ);
        m_style = (::fwGui::dialog::ILocationDialog::Options) (m_style | ::fwGui::dialog::ILocationDialog::WRITE);
    }
    else if ( option == ::fwGui::dialog::ILocationDialog::READ )
    {
        m_style = (::fwGui::dialog::ILocationDialog::Options) (m_style & ~::fwGui::dialog::ILocationDialog::WRITE);
        m_style = (::fwGui::dialog::ILocationDialog::Options) (m_style | ::fwGui::dialog::ILocationDialog::READ);
    }
    else if ( option == ::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST )
    {
        m_style =
            (::fwGui::dialog::ILocationDialog::Options) (m_style |
                                                         ::fwGui::dialog::ILocationDialog::FILE_MUST_EXIST);
    }

    return *this;
}

//------------------------------------------------------------------------------

// exemple ( addFilter("images","*.png *.jpg");
void LocationDialog::addFilter(const std::string &filterName, const std::string &wildcardList )
{
    m_filters.push_back( std::make_pair( filterName, wildcardList ));
}

//------------------------------------------------------------------------------

// "BMP and GIF files (*.bmp *.gif)|*.bmp *.gif|PNG files (*.png)|*.png"
QString LocationDialog::fileFilters()
{
    std::string result;
    std::vector< std::pair < std::string, std::string > >::const_iterator iter;
    for ( iter = m_filters.begin(); iter!= m_filters.end(); ++iter)
    {
        std::string filterName   = iter->first;
        std::string rawWildcards = iter->second;

        if (iter!=m_filters.begin() )
        {
            result += ";;";
        }
        result += filterName +" (" +  rawWildcards +")";
    }
    return QString::fromStdString(result);
}

//------------------------------------------------------------------------------
} // namespace dialog
} //namespace fwGuiQt
