<br/>
<br/>

<center> <font size = 5> 哈尔滨工业大学计算机科学与技术学院 </font></center>

<br/>
<br/>

<center> <font size = 7> 实验报告 </font></center>

<br/>
<br/>
<br/>

<center> <font size = 5> 
课程名称：机器学习 <br/>
课程类型：必修  <br/>
实验题目：逻辑回归
</font></center>


<br/>
<br/>

<center> <font size = 4> 学号：1190200523 </font></center>
<center> <font size = 4> 姓名：石翔宇 </font></center>

<div STYLE="page-break-after: always;"></div>

<!-- 此处用于换行 -->

## 一、实验目的

理解逻辑回归模型，掌握逻辑回归模型的参数估计算法。

## 二、实验要求及实验环境

### 实验要求

实现两种损失函数的参数估计（1. 无惩罚项；2. 加入对参数的惩罚），可以采用梯度下降、共轭梯度或者牛顿法等。

#### 验证方法

1. 可以手工生成两个分别类别数据（可以用高斯分布），验证你的算法。考察类条件分布不满足朴素贝叶斯假设，会得到什么样的结果。
2. 逻辑回归有广泛的用处，例如广告预测。可以到UCI网站上，找一实际数据加以测试。

### 实验环境

Windows 11 + Python 3.7.8

## 三、设计思想

### 1. 逻辑回归

逻辑回归又名对数几率回归，虽然名字叫“回归”，但实际上是一种分类学习方法。

考虑广义线性模型
$$
y=g(w^Tx+b)
$$
，除了可以做回归问题，还可以做分类任务，只需找到一个单调可微函数将分类任务的真是标记$y$与线性回归模型的预测值联系起来。

考虑连续可微的对数几率函数 
$$
y=\frac{1}{1+e^{-z}}
$$
将其代入式 1 得到
$$
y=\frac{1}{1+e^{-(w^Tx+b)}}
$$
将其取对数，则可以变化为
$$
\ln\frac{y}{1-y}=w^Tx+b
$$
若将$y$视为样本$x$作为正例的可能性，则$1-y$是其反例可能性，则称$\frac{y}{1-y}$为几率，$\ln\frac{y}{1-y}$为对数几率。实际上，就是在用线性回归模型的预测结果去逼近真实标记的对数几率。

现在我们来确定式 3 中的$w$和$b$。

我们将式 3 中的 $y$ 视为类后验概率估计 $p(y=1|x)$ ，则式 4 可重写为
$$
\ln\frac{p(y=1|x)}{p(y=0|x)}=w^Tx+b
$$
由式 3 可得
$$
p(y=1|x)=\frac{e^{w^Tx+b}}{1+e^{w^Tx+b}}\\
p(y=0|x)=\frac{1}{1+e^{w^Tx+b}}\\
$$
于是，我们可以用极大似然法估计$w$和$b$。给定数据集$\{(x_i,y_i\}^m_{i=1}$，为便于讨论，令$\beta=(w;b),\hat{x}=(x;1)$，则$w^Tx+b=\beta^T\hat{x}$。则对数几率回归模型最大化对数似然为
$$
l(w,b)=&\sum_{i=1}^{m}\ln p(y_i|x_i;w,b)\\
$$
要将$l(w,b)$最大化，也即最小化
$$
\begin{aligned}
J(\beta)=&-\frac{1}{m}\sum_{i=1}^{m}\ln ({y_i\frac{e^{\beta^T\hat{x}_i}}{1+e^{\beta^T\hat{x}_i}}+ ( 1-y_i)\frac{1}{1+e^{\beta^T\hat{x}_i}}})\\
	  =&-\frac{1}{m}\sum_{i=1}^{m}\ln \frac{y_ie^{\beta^T\hat{x}_i}+1-y_i}{1+e^{\beta^T\hat{x}_i}}\\
	  =&-\frac{1}{m}\sum_{i=1}^{m}({\ln (y_ie^{\beta^T\hat{x}_i}+1-y_i)}-\ln(1+e^{\beta^T\hat{x}_i}))\\
	  =&\frac{1}{m}\sum_{i=1}^{m}(-{y_i\beta^T\hat{x}_i+\ln(1+e^{\beta^T\hat{x}_i})})\\
\end{aligned}
$$
则我们的任务目标就变为
$$
\min_\beta J(\beta)
$$
若我们加入正则项，则有
$$
J'(\beta)=J(\beta)+\frac{\lambda}{2}\|\beta\|^2_2
$$
相应地，加入正则项后的任务目标变为
$$
\min_\beta J'(\beta)
$$

### 2. 梯度下降法（无正则项）

$J(\beta)$是关于$\beta$的高阶可导连续凸函数，则可用梯度下降法求导其最优解。

我们将$J(\beta)$对$\beta$求偏导可得
$$
\frac{\partial J(\beta)}{\partial\beta}=\frac{1}{m}\sum_{i=1}^m\hat{x}_i(-y_i+\frac{e^{\beta^T\hat{x}_i}}{1+e^{\beta^T\hat{x}_i}})=\nabla J(\beta)
$$
则按照梯度下降法，每一步我们都按照
$$
\beta_{i+1}=\beta_i-\alpha \nabla J(\beta)
$$
来更新 $\beta$ ，其中 $\alpha$ 被称作为学习率或者步长，满足 $\alpha > 0$ 。

### 3. 梯度下降法（有正则项）

相应地，加入正则项后，我们将$J'(\beta)$对$\beta$求偏导可得
$$
\frac{\partial J'(\beta)}{\partial\beta}=\frac{1}{m}\sum_{i=1}^m\hat{x}_i(-y_i+\frac{e^{\beta^T\hat{x}_i}}{1+e^{\beta^T\hat{x}_i}})+{\lambda}\beta=\nabla J'(\beta)
$$
每一步我们按照
$$
\beta_{i+1}=\beta_i-\alpha \nabla J'(\beta)
$$
来更新 $\beta$ 。

### 4. 牛顿法（无正则项）

$J(\beta)$是关于$\beta$的高阶可导连续凸函数，则也可用牛顿法求导其最优解。

假设 $\beta$ 的第$k$次迭代值为$\beta^{(k)}$，则可将$J(\beta)$在$\beta^{(k)}$附近进行二阶泰勒展开
$$
J(\beta)=J(\beta^{(k)})+g_k^T(\beta-\beta^{(k)})+\frac{1}{2}(\beta-\beta^{(k)})^TH(\beta^{(k)})(\beta-\beta^{(k)})
$$
其中$g_k=g(\beta^{(k)})=\nabla J(\beta^{(k)})$ 是$J(\beta)$的梯度向量在点$\beta^{(k)}$的值，$H(\beta^{(k)})$是$J(\beta)$的黑塞矩阵
$$
\begin{aligned}
H(\beta)=&\begin{bmatrix}\frac{\partial^2J}{\partial \beta_i \partial\beta_j}\end{bmatrix}_{n\times n}\\
		=&\frac{\partial^2J}{\partial \beta \partial\beta^T}\\
		=&\frac{1}{m}\sum_{i=1}^{m}\hat{x}_i\hat{x}_i^T\frac{e^{\beta^T\hat{x}_i}}{(1+e^{\beta^T\hat{x}_i})^2}\\
\end{aligned}
$$
在点$\beta^{(k)}$的值。函数$J(\beta)$有极值的必要条件是在极值点处梯度为$0$，即
$$
\nabla J(\beta)=0
$$
假设第$k+1$次迭代时有$\nabla J(\beta^{(k+1)})=0$，则由式 16 可得
$$
\nabla J(\beta)=g_k+H_k(\beta^{(k+1)}-\beta^{(k)})=0
$$
则迭代式为
$$
\beta^{(k+1)}=\beta^{(k)}-H^{-1}_kg_k
$$

### 4. 牛顿法（有正则项）

相应地，加入正则项后，$J'(\beta)$的黑塞矩阵为
$$
\begin{aligned}
H'(\beta)=&\begin{bmatrix}\frac{\partial^2J'}{\partial \beta_i \partial\beta_j}\end{bmatrix}_{n\times n}\\
		=&\frac{\partial^2J'}{\partial \beta \partial\beta^T}\\
		=&\frac{1}{m}\sum_{i=1}^{m}\hat{x}_i\hat{x}_i^T\frac{e^{\beta^T\hat{x}_i}}{(1+e^{\beta^T\hat{x}_i})^2}+\lambda\\
\end{aligned}
$$
则迭代式为
$$
\beta^{(k+1)}=\beta^{(k)}-H'^{-1}_kg'_k
$$


## 四、实验结果分析

### 1. 手工数据集生成

我们将正例和负例的均值分别设置为$(1,3)$和$(2,5)$，标准差都设置为$0.2$，相关系数为$0.1$或$0$（前者为不满足朴素贝叶斯假设时，后者为满足时）。训练集和测试集的个数分别设置为$200$和$300$（以下所有测试的数据皆是如此）。下面分别图示了两个集合的分布：

![ml_lab2_1](D:\Dedsecr\HIT-MachineLearning\labs\lab2\images\ml_lab2_1.png)

### 2. 梯度下降法在不满足朴素贝叶斯假设的手工数据集上的测试

在不满足朴素贝叶斯假设时，正例和负例的相关系数为$0.1$。我们将最大迭代次数设置为$70000$，学习率设置为$10^{-1}$，早停策略的参数值设置为$10^{-7}$，正则项系数设置为$10^{-3}$（若有的话）。无正则项和有正则项时的测试结果如下图：

![ml_lab2_2](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_2.png)

可以看到，正则项对分类结果影响并不大，但是对收敛速度影响很大。

### 2. 梯度下降法在满足朴素贝叶斯假设的手工数据集上的测试

在满足朴素贝叶斯假设时，正例和负例的相关系数为$0$。我们将最大迭代次数设置为$70000$，学习率设置为$10^{-1}$，早停策略的参数值设置为$10^{-7}$，正则项系数设置为$10^{-3}$（若有的话）。无正则项和有正则项时的测试结果如下图：

![ml_lab2_3](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_3.png)

可以看到，正则项对分类结果影响并不大，但是对收敛速度影响较大。

### 3. 牛顿法在不满足朴素贝叶斯假设的手工数据集上的测试

在满足朴素贝叶斯假设时，正例和负例的相关系数为$0$。我们将最大迭代次数设置为$70000$，早停策略的参数值设置为$10^{-7}$，正则项系数设置为$10^{-3}$（若有的话）。无正则项和有正则项时的测试结果如下图：

![ml_lab2_4](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_4.png)

可以看到，正则项对分类结果影响并不大，但是对收敛速度影响很大。

### 4. 牛顿法在满足朴素贝叶斯假设的手工数据集上的测试

在满足朴素贝叶斯假设时，正例和负例的相关系数为$0$。我们将最大迭代次数设置为$70000$，早停策略的参数值设置为$10^{-7}$，正则项系数设置为$10^{-3}$（若有的话）。无正则项和有正则项时的测试结果如下图：

![ml_lab2_5](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_5.png)

可以看到，正则项略微提升了分类性能，对收敛速度影响很大。

### 4. 手工数据集上的测试对比

通过上面的测试我们可以看出，满足朴素贝叶斯假设与否，加入正则项与否，都对分类性能的影响不太大。与梯度下降法相比，牛顿法收敛速度更快；并且可以看出，在牛顿法中加入正则项对收敛速度的提升较大。

### 5. UCI数据上的测试

我们使用UCI网站上的数据集[Skin Segmentation Data Set](http://archive.ics.uci.edu/ml/datasets/Skin+Segmentation)。该数据集大小为245057，维度为4。我们随机选择2000个数据（1000个正例，1000个负例），将其按照$30\%、70\%$ 的比例划分为训练集和测试集，训练集中有600个数据，测试集中有1400个数据。图示如下：

![ml_lab2_6_1](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_6_1.png)

![ml_lab2_6_2](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_6_2.png)

我们利用梯度下降法测试，最大迭代次数设置为$7000$，学习率设置为$10^{-4}$，测试结果如下：

![ml_lab2_7_1](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_7_1.png)

![ml_lab2_7_2](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_7_2.png)

我们利用牛顿法测试，最大迭代次数设置为$70000$，测试结果如下：

![ml_lab2_8_1](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_8_1.png)

![ml_lab2_8_2](https://gitee.com/Dedsecr/pic/raw/master/pic/ml_lab2_8_2.png)

通过上面的测试我们可以看出，分类器在真实数据集上也能够表现良好。

## 五、结论

在求解逻辑回归问题中，我们用到了两种方法，分别是梯度下降法和牛顿法。梯度下降法每次迭代较快，但是所需迭代次数较多；牛顿法每次迭代较慢，但所需迭代次数较少。总体来说，牛顿法较快，但是适用范围较小，当黑塞矩阵奇异时，牛顿法不再适用。

对于数据是否满足朴素贝叶斯假设对性能的影响，从上述实验可以看出，是否满足假设影响不大。

## 六、参考文献

https://timvieira.github.io/blog/post/2014/02/11/exp-normalize-trick/



## 七、附录:源代码(带注释)

