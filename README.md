# SegmenTree

Using class and pointer to make a segment tree to maintain the data.

However, this datastructure is still developing, 'cause there are many unanticipated realizations.

# How to use

There are two calculation options you can choose: **without mod** or **within mod**. You just need to change `modOp` from `true` to `false` or change it from `false` to `true`. Also, you can change the mod number while you use mod calculation.

When you need to maintain the **minimum data** or the **maximum data**, you can update the infinite value `infval` to make sure that your data range is inside that.

You can create an object of class `SegmentTree`, and you need put your data ( in an array or a vector ) into the construct function.

There are two options for your data: one is `UpdateValue`, another is `QueryValue`. They are done in consecutive ranges.

## Maintain your data

The two member functions `UpdateValue` and `QueryValue` have some options to choose. You can use different options by passing in the parameters `Option`.

- `UpdateValue(int RangeLeft, int RangeRight, T ArgValue, int Option)`
  `Option = 1`: you can change the data value from `RangeLeft` to `RangeRight` by adding `ArgValue` to them;
  `Option = 2`: you can change the data value from `RangeLeft` to `RangeRight` by multiplying `ArgValue` to them.

- `QueryValue(int RangeLeft, int RangeRight, int Option)`

  `Option = 1`: you can get the sum of data value from `RangeLeft` to `RangeRight`;
  `Option = 2`: you can get the minimum data value from `RangeLeft` to `RangeRight`;
  `Option = 3`: you can get the maximum data value from `RangeLeft` to `RangeRight`.

## One Example

The following program will show you how to create a segment tree and use it to maintain your data.

```cpp
// Paste the whole code in here from my github program "SegmentTree.cpp"

int main()
{
    int n;
    cin >> n;
    vector<int> MyData(n);
    for(int i = 0; i < n; i += 1)
    	cin >> MyData[i];
    SegmentTree<int> MyTree(MyData);
    MyTree.UpdateValue(1, n>>1, 2, 1);
    MyTree.UpdateValue(1, n, 3, 2);
    cout << "The sum of all data now is : " << MyTree.QueryValue(1, n, 1) << endl;
    cout << "The minimum data now is : " << MyTree.QueryValue(1, n, 2) << endl;
    cout << "The maximum data now is : " << MyTree.QueryValue(1, n, 3) << endl;
    return 0;
}
```

# Problems to be figured out

The `EndNode` cannot change its type while you create different type of segment tree to maintain different type of data. I need time to find out how to solve it.

After that, Some data protection problems are also about to be done.

All of these are just my immature try, please forgive me if you find that there are too many problems here.
