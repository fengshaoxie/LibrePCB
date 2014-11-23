/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
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
#include "ses_drawtext.h"
#include "../schematiceditor.h"
#include "ui_schematiceditor.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

SES_DrawText::SES_DrawText(SchematicEditor& editor, Ui::SchematicEditor& editorUi) :
    SchematicEditorState(editor, editorUi)
{
}

SES_DrawText::~SES_DrawText()
{
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

SchematicEditorState::State SES_DrawText::process(SchematicEditorEvent* event) noexcept
{
    SchematicEditorState::State nextState = State_DrawText;

    switch (event->getType())
    {
        case SchematicEditorEvent::AbortCommand:
        case SchematicEditorEvent::StartSelect:
            nextState = State_Select;
            break;

        case SchematicEditorEvent::StartMove:
            nextState = State_Move;
            break;

        case SchematicEditorEvent::StartDrawRect:
            nextState = State_DrawRect;
            break;

        case SchematicEditorEvent::StartDrawPolygon:
            nextState = State_DrawPolygon;
            break;

        case SchematicEditorEvent::StartDrawCircle:
            nextState = State_DrawCircle;
            break;

        case SchematicEditorEvent::StartDrawEllipse:
            nextState = State_DrawEllipse;
            break;

        case SchematicEditorEvent::StartDrawWire:
            nextState = State_DrawWire;
            break;

        case SchematicEditorEvent::StartAddComponent:
            nextState = State_AddComponent;
            break;

        case SchematicEditorEvent::SwitchToSchematicPage:
        {
            SEE_SwitchToSchematicPage* e = dynamic_cast<SEE_SwitchToSchematicPage*>(event);
            Q_CHECK_PTR(e); Q_UNUSED(e);
            e->setAccepted(true);
            break;
        }

        default:
            break;
    }

    return nextState;
}

void SES_DrawText::entry(State previousState) noexcept
{
    Q_UNUSED(previousState);

    mEditorUi.actionToolDrawText->setCheckable(true);
    mEditorUi.actionToolDrawText->setChecked(true);
}

void SES_DrawText::exit(State nextState) noexcept
{
    Q_UNUSED(nextState);

    mEditorUi.actionToolDrawText->setCheckable(false);
    mEditorUi.actionToolDrawText->setChecked(false);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
