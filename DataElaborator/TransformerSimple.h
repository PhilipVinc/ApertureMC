//
//  TransformerSimple.h
//  DataElaborator
//
//  Created by Filippo Vicentini on 19/03/14.
//  Copyright (c) 2014 Filippo Vicentini. All rights reserved.
//

#ifndef __DataElaborator__TransformerSimple__
#define __DataElaborator__TransformerSimple__

#include "DataSet.h"

class TransformerSimple
{
public:
    static void ShiftX(DataSet * data, long double value);
    static void ShiftY(DataSet * data, long double value);
    static void ScaleX(DataSet * data, long double value);
    static void ScaleY(DataSet * data, long double value);
};

#endif /* defined(__DataElaborator__TransformerSimple__) */
