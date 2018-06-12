# /usr/bin/evn python
# -*-coding:utf-8-*-
"""
    __author__:sa
"""
import pandas as pd
import numpy as np

dates = pd.date_range('20130101', periods=6, freq='T')  # 以20130101为基点向后生产时间点
# freq 频率,采样频率为一分钟。
print dates
df = pd.DataFrame({'A': pd.date_range('20130101', periods=4, freq='T'),
                   'B': pd.Timestamp('20130101'),
                   'C': np.array([3] * 4, dtype='int32'),
                   'D': pd.Series(1, index=list(range(4)), dtype='float'),
                   'E': pd.Categorical(['test1', 'test2', 'test3', 'test4']),
                   'F': list('foot')})

print df
print df.dtypes
print df.index
print df.columns

print df.head(2)
print df.head()
print df.tail(3)
print df.describe()
print df.sort_index(axis=1, ascending=False)
print df.sort(columns='A', ascending=False)
# 当然获取某列数据最直接的方式是df.[列标签]，但是当列标签未知时可以通过这种方式获取列数据。需要注意的是，dataframe的索引[1:3]是包含1, 2, 3的，与平时的不同。
print df.loc[0]  # loc——通过行标签索引行数据(index)
print df.loc[1:]  # loc可以获取多行数据
print df.loc[1, ['A', 'B']]  # loc扩展——索引某行某列
print df.loc[:, ['C']]  # loc扩展——索引某列
# iloc——通过行号获取行数据
print df.iloc[1]
# ix——结合前两种的混合索引
print df.ix[1]

print df.at[1, 'A']  # 快速访问一个标量（与上一个方法等价
df.at['a', 'C'] = 0
# 通过标签设置新的值
df.iat[4, 0] = 0
# 通过位置设置新的值,如果dtypes不是int新添加的值会随dtypes更改
df.loc[:, ['D']] = np.array([5] * len(df))
print df, 'len:', len(df)
df2 = df.copy(deep=True)
df2.dropna(how='any', axis=1)  # axis : {0 or ‘index’, 1 or ‘columns’}
# how : {‘any’, ‘all’}
# any : if any NA values are present, drop that label
# all : if all values are NA, drop that label
print df2, 'shape:', df2.shape, df2.notnull(), df.isnull()
s = pd.Series([1, 3, 5, np.nan, 6, 8], index=dates).shift(2)
# 生成序列并向右平移两位
print df2, df2.mean(1)  # 仅计算dtypes为数值类的列或列，默认是列
print s
print '----------------------->'
print 'sub:\n', df2['D'].sub(s)
print '----------------------->'
print df['D'].apply(np.cumsum)  # 对数据应用函数：
print df['D'].value_counts()  # 计算数值对应的数量
print df['F'].str.upper()  # Series对象在其str属性中配备了一组字符串处理方法，可以很容易的应用到数组中的每个元素

pieces = [df[:1], df[1:3], df[3:]]
print pd.concat(pieces)
# print pd.merge(df,s,on='C')
# print df.append(s,ignore_index=True)

# 对于”group by”操作，我们通常是指以下一个或多个操作步骤：
#
# l  （Splitting）按照一些规则将数据分为不同的组；
#
# l  （Applying）对于每组数据分别执行一个函数；
#
# l  （Combining）将结果组合到一个数据结构中；
print df2.groupby(['A', 'C']).sum()
# 透视表
# df = df['A'].astype(str)
table = pd.pivot_table(df, index=["A", "B"], columns=["E"], values=["C", "D"],
                       aggfunc={"C": len, "D": [np.sum, np.mean]}, fill_value=0)

print table
# stack 过程表示将数据集的列(columns)旋转为行，同理 unstack 过程表示将数据的行(index)旋转为列。
# stack 使得数据集变得更长，unstack 使得数据集变得更宽。
print table.stack()
print table.unstack(1)
# resample，重新采样，是对原样本重新处理的一个方法，是一个对常规时间序列数据重新采样和频率转换的便捷的方法。
index = pd.date_range('1/1/2000', periods=9, freq='T')
series = pd.Series(range(9), index=index)
print 'resample:', series.resample('1T').sum()
# 降低采样频率为三分钟，但是每个标签使用right来代替left,但是关闭right区间
print series.resample('3T', label='right', closed='right').sum()
# print series.resample('30S').asfreq()[0:5]
# 增加采样频率到30S,使用pad方法填充nan值。
# print series.resample('30S').pad()[0:5]

# Categorical按某一列重新编码分类
# 对a中的gender进行重新编码分类，将对应的0，1转化为male，female
df['F'] = df['F'].astype('category')
df['F'].cat.categories = [1, 2, 3]

print 'categories:\n', df

