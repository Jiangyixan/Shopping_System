# Shopping_System
## 前言
北邮c++大作业电商交易平台网络版， 大二的大作业了也是。题目一和题目二是完整版的功能实现，不过题目三是网络版，在原单机版的情况下加上了server-client的socket通信，所以网络版实现的功能就较少了。需要看单机版的全部功能移步：[电商管理系统单机版](https://github.com/Jiangyixan/Shopping-System-StandaloneVersion)

## 🛠️ 技术栈
- VS(主要）
- 网络库：Winsock2
- json,支持json格式
## 正文
网络版主要实现的功能就是：登录、展示平台商品信息、搜索平台商品信息、购物车管理、订单生产、订单支付
### 系统采用面向对象的设计方法，主要包含以下几个核心模块：
- 用户管理模块（User）
- 商品管理模块（Product）
- 购物车模块（ShoppingCart）
- 数据处理模块（DataHandler）
### 类关系说明：
**1. 继承关系：**
   - User <- Consumer/Seller
   - Product <- Book/Food/Clothing
**2. 组合关系：**
   - ShoppingCart 包含 CartItem 对象
   - Product 关联 User（通过 ownername）
**3. 依赖关系：**
   - ShoppingCart 依赖 User（结算时）
   - DataHandler 依赖 User 和 Product（数据管理）
### 主要属性说明：
**1. User类：**
   - username：用户名
   - password：密码
   - balance：账户余额

**2. Product类：**
   - id：商品ID
   - name：商品名称
   - description：商品描述
   - price：价格
   - quantity：库存数量
   - ownername：商品所有者
**3. CartItem类：**
   - cartItemId：购物车项ID
   - productName：商品名称
   - quantity：数量
   - seller：卖家
   - price：价格
**4. ShoppingCart类：**
   - items：购物车项列表
   - nextItemId：下一个购物车项ID
   - username：用户名
### 图示
***整体系统图***

![image](https://github.com/user-attachments/assets/cc8cb39f-d098-4d71-88e8-50b3c0fe227f)

**1.类关系说明**
    - User类作为基类，定义了用户的基本属性和方法
    - Consumer和Seller类继承自User类，分别实现消费者和商家的特定功能
    - 所有用户共享基本的账户管理功能（充值、消费、密码修改等）
    
![用户说明图](https://github.com/user-attachments/assets/ade5d71a-5e5d-4bb4-87bb-abf3549fb55f)

**2. 类关系说明**
    - Product类作为基类，定义了商品的基本属性和方法
    - Book、Food、Clothing类继承自Product类，实现不同类型商品的特定功能
    - 每种商品类型可以有自己的价格计算策略

**2.1 关键方法说明**
    - getPrice()：获取商品价格（可被子类重写实现不同的价格策略）
    - getProductType()：获取商品类型
    - updatePrice()：更新商品价格
    - updateQuantity()：更新商品库存

![商品说明图](https://github.com/user-attachments/assets/2686f018-c9fe-4d5e-a157-807c919ccbd5)

**3.类关系说明**
    - ShoppingCart类管理购物车整体功能
    - CartItem类表示购物车中的单个商品项
    - ShoppingCart通过vector容器管理多个CartItem对象
    **3.1 关键方法说明**
    - addItem()：添加商品到购物车
    - removeItem()：从购物车移除商品
    - updateItemQuantity()：更新商品数量
    - checkout()：结算购物车
    - calculateTotal()：计算总金额

![购物车说明图](https://github.com/user-attachments/assets/d89ee5ae-b6d4-4993-a1b5-4c2234bf0ccf)

**4.类关系说明**
    - DataHandler类负责所有数据的持久化操作
    - 与User类和Product类交互，处理用户和商品数据的存储和读取
    - 提供数据查询和更新接口

   **4.1关键方法说明**
    - saveUser()/loadUsers()：用户数据的保存和加载
    - saveProduct()/loadProducts()：商品数据的保存和加载
    - loginUser()：用户登录验证
    - getProductByXXX()：多种商品查询方式
    - updateProduct()/deleteProduct()：商品信息更新和删除
、
![image](https://github.com/user-attachments/assets/b0c924c8-b7d5-4964-98ef-5f671b5105e3)

