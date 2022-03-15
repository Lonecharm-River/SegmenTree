# SegmenTree
Using class and pointer to make a segment tree to maintain the data.

However, this datastructure is still developing, 'cause there are many unanticipated realizations.

# How to use
There are two calculation options you can choose: **without mod** or **within mod**. You just need to change `modOp` from `true` to `false` or change it from `false` to `true`. Also, you can change the mod number while you use mod calculation.

When you need to maintain the **minimum data** or the **maximum data**, you can update the infinite value `infval` to make sure that your data range is inside that.

## Maintain your data

You can create one member of class `SegmentTree`, and you need put your data ( in an array or a vector ) into the construct function.

There are two options for your data: one is `UpdateValue`, another is `QueryValue`. They are done in consecutive ranges.

# Problems to be figured out

The `EndNode` cannot change its type while you create different type of segment tree to maintain different type of data. I need time to find out how to solve it.

After that, Some data protection problems are also about to be done.

All of these are just my non-matual try, please forgive me if you find that there are too many problems here.
