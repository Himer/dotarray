# dotarray
>皆知PHP的数组是由HashTable和双链表实现的，为了方便大家查看数组的数据结构，开发一个遍历PHP的数组生成Dot描述的PHP插件，生成dot描述以后可以通过一些渲染工具生成图像，本例用的是 Graphviz<br>
   
扩展里边函数说明 <br>
<br>--------------------------------------------------------------------------------------------- <br>
string    dotarray( array $input [, int $flag] ) <br>
生成数组数据结构的dot描述 <br>
参数 <br>
input   需要操作的数组 <br>
flag   查看那些数据结构，是扩展提供的3个常量或操作，分别是 <br>
     DOTARAAY_HASH_TABLE 表示显示HashTable结构 <br>
     DOTARRAY_DOUBLE_LIST 显示数组的双链表结构 <br>
     DOTARRAY_CURRENT_POSITION 显示数组现在的内部指针的位置 <br>
返回值 <br>
成功返回dot描述字符串，错误（非数组等情况）返回false 
<br>--------------------------------------------------------------------------------------------- <br>
显示双链表结构例子 <br>

<pre><code>
<?php  
  
$items = array(1,2,8=>'lalala',16=>'hahaha','name'=>'shiki',30=>'wooooo...');  
next($items);/*将内部指针移到下一位*/  
$result = dotarray($items,DOTARRAY_DOUBLE_LIST|DOTARRAY_CURRENT_POSITION);  
echo  $result;  
</code></pre>
得到的dot描述： 

<pre><code>
digraph html {label = "Structure of array";  
node[shape = record];  
===========部分内容省略============  
edge [color=black];  
edge [color=green];  
sk_array:f5:s -> sk_item_1:f0;  
edge [color=black];  
}  
</code></pre>

通过graphviz渲染得到下面的图片 

![](http://dl2.iteye.com/upload/attachment/0107/2260/10f186b3-e037-3772-93d3-ea06609e6c89.png)
