//
//  Created by Asger Nyman Christiansen on 08/01/2017.
//  Copyright © 2017 Asger Nyman Christiansen. All rights reserved.
//

#pragma once

namespace gle
{
    class GLState
    {
    public:
        static void cull_back_faces(bool enable)
        {
            static bool currently_enabled = true;
            if(currently_enabled != enable)
            {
                if(enable)
                {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }
                else {
                    glDisable(GL_CULL_FACE);
                }
                currently_enabled = enable;
            }
        }
        
        static void depth_write(bool enable)
        {
            static bool currently_enabled = true;
            if(currently_enabled != enable)
            {
                if(enable)
                {
                    glDepthMask(GL_TRUE);
                }
                else {
                    glDepthMask(GL_FALSE);
                }
                currently_enabled = enable;
            }
        }
    };
}
