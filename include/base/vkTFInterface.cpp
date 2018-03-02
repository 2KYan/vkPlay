/*=======================================================================
    This file is part of vkTF

    vkTF is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    vkTF is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/
//=======================================================================
/*
    Descriptions:

    Notes:

    Authors:
        2KYan, 2KYan@outlook.com

*/
//=======================================================================
//

#include "vkTFInterface.h"
#include "vulkanexamplebase.h"

int VulkanExampleBase::vkTFInit(void*)
{
    initVulkan();
    setupWindow(GetModuleHandle(0), VulkanExampleBase::WndProc);
    prepare();
    printf("Init\n");
    return 0;
}

int VulkanExampleBase::vkTFRender(int)
{
    renderLoop();
    printf("Render\n");
    return 0;
}

int VulkanExampleBase::vkTFExit(void)
{
    printf("Exit\n");
    return 0;
}
