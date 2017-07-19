//
//  BGTool.h
//  BGFMDB
//
//  Created by huangzhibiao on 17/2/16.
//  Copyright © 2017年 Biao. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import "BGModelInfo.h"
#import "FMDB.h"

#define SQLITE_NAME @"BGFMDB.sqlite"

// 过期
#define BGFMDBDeprecated(instead) NS_DEPRECATED(2_0, 2_0, 2_0, 2_0, instead)

// 日志输出
#ifdef DEBUG
#define BGLog(...) NSLog(__VA_ARGS__)
#else
#define BGLog(...)
#endif

#define debug(param) do{\
if(self.debug){BGLog(@"调试输出: %@",param);}\
}while(0)

#define BG @"BG_"
#define BGPrimaryKey @"bg_id"
#define BGCreateTime @"bg_createTime"
#define BGUpdateTime @"bg_updateTime"

#define Complete_B void(^_Nullable)(BOOL isSuccess)
#define Complete_I void(^_Nullable)(bg_dealState result)
#define Complete_A void(^_Nullable)(NSArray* _Nullable array)
#define ChangeBlock void(^_Nullable)(bg_changeState result)

typedef NS_ENUM(NSInteger,bg_changeState){//数据改变状态
    bg_insert,//插入
    bg_update,//更新
    bg_delete,//删除
    bg_drop//删表
};

typedef NS_ENUM(NSInteger,bg_dealState){//处理状态
    bg_error = -1,//处理失败
    bg_incomplete = 0,//处理不完整
    bg_complete = 1//处理完整
};

typedef NS_ENUM(NSInteger,bg_sqliteMethodType){//处理状态
    bg_min,//求最小值
    bg_max,//求最大值
    bg_sum,//求总和值
    bg_avg//求平均值
};

//keyPath查询用的关系，bg_equal:等于的关系；bg_contains：包含的关系.
typedef NSString* _Nonnull bg_relation;
extern bg_relation const bg_equal;
extern bg_relation const bg_contains;
@interface BGTool : NSObject

/**
 封装处理传入数据库的key和value.
 */
extern NSString* _Nonnull bg_sqlKey(NSString* _Nonnull key);
extern NSString* _Nonnull bg_sqlValue(id _Nonnull value);
/**
 根据keyPath和Value的数组, 封装成数据库语句，来操作库.
 */
extern NSString* _Nonnull bg_keyPathValues(NSArray* _Nonnull keyPathValues);

/**
 自定义数据库名称.
 */
extern void bg_setSqliteName(NSString*_Nonnull sqliteName);
/**
 删除数据库文件
 */
extern BOOL bg_deleteSqlite(NSString*_Nonnull sqliteName);
/**
 设置操作过程中不可关闭数据库(即closeDB函数无效).
 默认是NO.
 */
extern void bg_setDisableCloseDB(BOOL disableCloseDB);
/**
 设置调试模式
 @debug YES:打印调试信息, NO:不打印调试信息.
 */
extern void bg_setDebug(BOOL debug);


/**
 json字符转json格式数据 .
 */
+(id _Nonnull)jsonWithString:(NSString* _Nonnull)jsonString;
/**
 字典转json字符 .
 */
+(NSString* _Nonnull)dataToJson:(id _Nonnull)data;
/**
 根据类获取变量名列表
 @onlyKey YES:紧紧返回key,NO:在key后面添加type.
 */
+(NSArray* _Nonnull)getClassIvarList:(__unsafe_unretained _Nonnull Class)cla onlyKey:(BOOL)onlyKey;

/**
 抽取封装条件数组处理函数.
 */
+(NSArray* _Nonnull)where:(NSArray* _Nonnull)where;
/**
 封装like语句获取函数
 */
+(NSString* _Nonnull)getLikeWithKeyPathAndValues:(NSArray* _Nonnull)keyPathValues where:(BOOL)where;
/**
 判断是不是主键.
 */
+(BOOL)isUniqueKey:(NSString* _Nonnull)uniqueKey with:(NSString* _Nonnull)param;
/**
 判断并获取字段类型.
 */
+(NSString* _Nonnull)keyAndType:(NSString* _Nonnull)param;
/**
 根据类属性类型返回数据库存储类型.
 */
+(NSString* _Nonnull)getSqlType:(NSString* _Nonnull)type;
//NSDate转字符串,格式: yyyy-MM-dd HH:mm:ss
+(NSString* _Nonnull)stringWithDate:(NSDate* _Nonnull)date;
/**
 根据类属性值和属性类型返回数据库存储的值.
 @value 数值.
 @type 数组value的类型.
 @encode YES:编码 , NO:解码.
 */
+(id _Nonnull)getSqlValue:(id _Nonnull)value type:(NSString* _Nonnull)type encode:(BOOL)encode;
/**
 转换从数据库中读取出来的数据.
 @tableName 表名(即类名).
 @array 传入要转换的数组数据.
 */
+(NSArray* _Nonnull)tansformDataFromSqlDataWithTableName:(NSString* _Nonnull)tableName array:(NSArray* _Nonnull)array;
/**
 转换从数据库中读取出来的数据.
 @claName 类名.
 @valueDict 传入要转换的字典数据.
 */
+(id _Nonnull)objectFromJsonStringWithClassName:(NSString* _Nonnull)claName valueDict:(NSDictionary* _Nonnull)valueDict;
/**
 字典或json格式字符转模型用的处理函数.
 */
+(id _Nonnull)bg_objectWithClass:(__unsafe_unretained _Nonnull Class)cla value:(id _Nonnull)value;
/**
 模型转字典.
 */
+(NSMutableDictionary* _Nonnull)bg_keyValuesWithObject:(id _Nonnull)object ignoredKeys:(NSArray* _Nullable)ignoredKeys;
/**
 判断类是否实现了某个类方法.
 */
+(id _Nonnull)isRespondsToSelector:(SEL _Nonnull)selector forClass:(__unsafe_unretained _Nonnull Class)cla;
/**
 判断对象是否实现了某个方法.
 */
+(id _Nonnull)isRespondsToSelector:(SEL _Nonnull)selector forObject:(id _Nonnull)object;
/**
 根据对象获取要更新或插入的字典.
 */
+(NSDictionary* _Nonnull)getDictWithObject:(id _Nonnull)object ignoredKeys:(NSArray* const _Nullable)ignoredKeys isUpdate:(BOOL)update;
/**
 如果表格不存在就新建.
 */
+(BOOL)ifNotExistWillCreateTableWithObject:(id _Nonnull)object ignoredKeys:(NSArray* const _Nullable)ignoredKeys;
/**
 整形判断
 */
+ (BOOL)isPureInt:(NSString* _Nonnull)string;
/**
 浮点形判断
 */
+ (BOOL)isPureFloat:(NSString* _Nonnull)string;
/**
 NSUserDefaults封装使用函数.
 */
+(BOOL)getBoolWithKey:(NSString* _Nonnull)key;
+(void)setBoolWithKey:(NSString* _Nonnull)key value:(BOOL)value;
+(NSString* _Nonnull)getStringWithKey:(NSString* _Nonnull)key;
+(void)setStringWithKey:(NSString* _Nonnull)key value:(NSString* _Nonnull)value;
+(NSInteger)getIntegerWithKey:(NSString* _Nonnull)key;
+(void)setIntegerWithKey:(NSString* _Nonnull)key value:(NSInteger)value;
@end
