# dotarray
>皆知PHP的数组是由HashTable和双链表实现的，为了方便大家查看数组的数据结构，开发一个遍历PHP的数组生成Dot描述的PHP插件，生成dot描述以后可以通过一些渲染工具生成图像，本例用的是 Graphviz<br>
   
###扩展里边函数说明 
<pre>
--------------------------------------------------------------------------------------------- 
string    dotarray( array $input [, int $flag] ) 
生成数组数据结构的dot描述 
参数 
input   需要操作的数组 
flag   查看那些数据结构，是扩展提供的3个常量或操作，分别是 
     DOTARAAY_HASH_TABLE 表示显示HashTable结构 
     DOTARRAY_DOUBLE_LIST 显示数组的双链表结构 
     DOTARRAY_CURRENT_POSITION 显示数组现在的内部指针的位置 
返回值 
成功返回dot描述字符串，错误（非数组等情况）返回false 
--------------------------------------------------------------------------------------------- 
</pre>
###显示双链表结构例子 <br>

<pre>
$items = array(1,2,8=>'lalala',16=>'hahaha','name'=>'shiki',30=>'wooooo...');  
next($items);/*将内部指针移到下一位*/  
$result = dotarray($items,DOTARRAY_DOUBLE_LIST|DOTARRAY_CURRENT_POSITION);  
echo  $result;  
</pre>
###得到的dot描述： 

<pre>
digraph html {label = "Structure of array";  
node[shape = record];  
===========部分内容省略============  
edge [color=black];  
edge [color=green];  
sk_array:f5:s -> sk_item_1:f0;  
edge [color=black];  
}  
</pre>

###通过graphviz渲染得到下面的图片 

![](https://raw.githubusercontent.com/Himer/dotarray/master/dot.png)
