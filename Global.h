//
//  Global.h
//  Stromkreis
//
//  Created by Ujjwal Thaakar on 19/11/11.
//  Copyright 2011 Ujjwal Thaakar. All rights reserved.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef __clang__
#if __has_builtin(__builtin_expect)
#define DEFINED_EXPECTANCY
#define EXPECTED(cond) __builtin_expect(!!(cond), 1U)
#define UNEXPECTED(cond) __builtin_expect(!!(cond), 0U)
#endif
#endif

#ifdef __GNUC__
#ifndef DEFINED_EXPECTANCY
#define DEFINED_EXPECTANCY
#define EXPECTED(cond) __builtin_expect(!!(cond), 1U)
#define UNEXPECTED(cond) __builtin_expect(!!(cond), 0U)
#endif
#endif

#ifndef DEFINED_EXPECTANCY
#define DEFINED_EXPECTANCY
#define EXPECTED(cond) (cond)
#define UNEXPECTED(cond) (cond)
#endif

#endif // GLOBAL_H