/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "componentsymbolvariant.h"
#include "component.h"

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

ComponentSymbolVariant::ComponentSymbolVariant(const Uuid& uuid, const QString& norm,
                                               const QString& name_en_US,
                                               const QString& desc_en_US) noexcept :
    mUuid(uuid), mNorm(norm)
{
    Q_ASSERT(!mUuid.isNull());
    mNames.setDefaultValue(name_en_US);
    mDescriptions.setDefaultValue(desc_en_US);
}

ComponentSymbolVariant::ComponentSymbolVariant(const DomElement& domElement) throw (Exception)
{
    try
    {
        // read attributes
        mUuid = domElement.getAttribute<Uuid>("uuid", true);
        mNorm = domElement.getAttribute<QString>("norm", false);
        mNames.loadFromDomElement(domElement);
        mDescriptions.loadFromDomElement(domElement);

        // Load all symbol variant items
        foreach (const DomElement* node, domElement.getChilds("symbol_item")) {
            ComponentSymbolVariantItem* item = new ComponentSymbolVariantItem(*node);
            if (getItemByUuid(item->getUuid()))
            {
                throw RuntimeError(__FILE__, __LINE__,
                    QString(tr("The symbol variant item \"%1\" exists multiple times in \"%2\"."))
                    .arg(item->getUuid().toStr(), domElement.getDocFilePath().toNative()));
            }
            mSymbolItems.append(item);
        }

        if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);
    }
    catch (Exception& e)
    {
        qDeleteAll(mSymbolItems);       mSymbolItems.clear();
        throw;
    }
}

ComponentSymbolVariant::~ComponentSymbolVariant() noexcept
{
    qDeleteAll(mSymbolItems);       mSymbolItems.clear();
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void ComponentSymbolVariant::setNorm(const QString& norm) noexcept
{
    mNorm = norm;
}

void ComponentSymbolVariant::setName(const QString& locale, const QString& name) noexcept
{
    mNames.insert(locale, name);
}

void ComponentSymbolVariant::setDescription(const QString& locale, const QString& desc) noexcept
{
    mDescriptions.insert(locale, desc);
}

/*****************************************************************************************
 *  Symbol Item Methods
 ****************************************************************************************/

ComponentSymbolVariantItem* ComponentSymbolVariant::getItemByUuid(const Uuid& uuid) noexcept
{
    const ComponentSymbolVariant* const_this = this;
    return const_cast<ComponentSymbolVariantItem*>(const_this->getItemByUuid(uuid));
}

const ComponentSymbolVariantItem* ComponentSymbolVariant::getItemByUuid(const Uuid& uuid) const noexcept
{
    foreach (const ComponentSymbolVariantItem* item, mSymbolItems) {
        if (item->getUuid() == uuid)
            return item;
    }
    return nullptr;
}

QSet<Uuid> ComponentSymbolVariant::getAllItemSymbolUuids() const noexcept
{
    QSet<Uuid> set;
    foreach (const ComponentSymbolVariantItem* item, mSymbolItems) {
        set.insert(item->getSymbolUuid());
    }
    return set;
}

void ComponentSymbolVariant::addItem(ComponentSymbolVariantItem& item) noexcept
{
    Q_ASSERT(!mSymbolItems.contains(&item));
    mSymbolItems.append(&item);
}

void ComponentSymbolVariant::removeItem(ComponentSymbolVariantItem& item) noexcept
{
    Q_ASSERT(mSymbolItems.contains(&item));
    mSymbolItems.removeAll(&item);
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void ComponentSymbolVariant::serialize(DomElement& root) const throw (Exception)
{
    if (!checkAttributesValidity()) throw LogicError(__FILE__, __LINE__);

    root.setAttribute("uuid", mUuid);
    root.setAttribute("norm", mNorm);
    mNames.serialize(root);
    mDescriptions.serialize(root);
    serializePointerContainer(root, mSymbolItems, "symbol_item");
}

/*****************************************************************************************
 *  Private Methods
 ****************************************************************************************/

bool ComponentSymbolVariant::checkAttributesValidity() const noexcept
{
    if (mUuid.isNull())                     return false;
    if (mNames.getDefaultValue().isEmpty()) return false;
    if (mSymbolItems.isEmpty())             return false;
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb