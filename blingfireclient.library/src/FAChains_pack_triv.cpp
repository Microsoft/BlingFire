/**
 * Copyright (c) Microsoft Corporation. All rights reserved.
 * Licensed under the MIT License.
 */


#include "blingfire-client_src_pch.h"
#include "FAConfig.h"
#include "FAChains_pack_triv.h"
#include "BFendian.h"

namespace BlingFire
{

FAChains_pack_triv::FAChains_pack_triv () : 
    m_pImage (NULL),
    m_SizeOfValue (0),
    m_MaxCount (0)
{}


void FAChains_pack_triv::SetImage (const unsigned char * pImage)
{
    m_pImage = pImage;

    if (m_pImage) {

        m_SizeOfValue = from_le32(*(const int *)m_pImage);
        m_MaxCount = from_le32(*(const int *)(m_pImage + sizeof (int)));
    }
}

}
