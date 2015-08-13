V0.98(20140825)
1,Unify load tool DLL name 統一為 FactoryTool.dll
2,利用post message 區分出是使用EntranceTool開啟 (沿用原益眼綠)，或使用BAT開啟 (新底色)；並且在PICS L11 欄位填入 EntranceTool或BAT
3,按GO之後各page統一第一個Event都是 SetParameterValue("InitSlot", "i"), 除了OthersMultiDlg i從 1 開始，其餘從0開始

V0.97(20140818)
1,取消PreCheckMD5 function，使得開啟UI時不會修改到Tool MD5.xml的內容，確保Local Tool與Server Tool MD5一致，
使用Entrance tool開啟時，不需因UI更動到MD5檔案導致比對MD5失敗，而每次都重新拷貝Tool
2,修復InsertAndCheckAccessory使用的參數未設置預設值所導致的bug

V0.96(20140806)
1,增加功能，在FACTORY模式下，每次執行Tool都對該站的accessory 使用次數+1，並檢查accessory 使用次數是否已達到設定的最大值，若使用次數達到 (最大值-50)次，則顯示更換附件
  兩種方法設置AccessoryList:
  a.配合V2.018 以上的Entrance tool
  b.BAT檔帶的參數如下: UnifyUI_FacTestTool.exe [SO Number] [EmployeeID] [SW version] [Tool version] [Image Path] [Accessory List]
  Accessory List內容: AccessoryName:AccessoryID (Ex. USB:A11111,Headset:B11112)；Accessory List 未輸入亦不影響UI
2,解決 MMI_EM 出現請重測一次的問題

V0.95(20140715)
1,增加控制jig的command："out04on#"，"out04off#"
2,解决Multi Page在checkrouting 相关fail后，run之后又返回一次error code给UI的bug

V0.94(20140523)
1,IMEI page，导入系统设定id后，之前定义的imei buffer不够，会导致越界，造成memory内数据混乱。
现在修正UI涉及到的ID buffer都为60，解决此问题。
2,解决Factory版UI，从CIM query到的IDType和IDLength会被UIControl.ini的值给覆盖的问题。

V0.93(20140506)
1,OS_DL auto run时候，run fail后立马就给flag表示该slot可以接纳下一只手机，这时候如果立马链接另一只手机，如果post run比较久的话，
会导致前一只的post run没有执行完。
现在修改此flag在set result给UI后再变更，确保前一只手机的全部流程都执行结束。
2.IMEI/FCHECK id支持所有系统中可以query到的type(IMEI/ACER22/BTWifi/Lyson)

V0.92(20140318)
1,解决RD 版UI 在multi UI，在读到picasso为空时不判fail的问题。

V0.91(20140311)
1,升级DBAccess版本为1.082，解决'D'工单crash的问题

V0.90(20140224)
1,解决Multi UI 4个Device都设定成 NO USE (device = 0), UI停不下來
2,Multi UI执行最后原本会把picasso清空，现改为如果picasso不是扫描进入的，则测试结束后不会清空，下次run后，在getpicasso前清空
3,解决RD版Multi UI，读picasso fail后或者no use，DLL收到的SLOT_AVAILABLE_LIST 都是 1,1,1,1的错误
4,解决Multi UI重测没有不显示请重测字样
5,修改errorcode 由CommErr_UI_DL_Run_Thread_Fail变为CommErr_UI_Run_Thread_Fail

V0.89(20140210)
1,解决RD版下不能写FA的bug，只因判断可写FA条件多了一个segment为TLINE的条件，而RD版没有segment这个定义，所以删除这个条件。
2,COtherStationDlg.cpp中，ReadFA()里，读FA fail后，删除原本的postrun。会导致以postrun结果为实际结果的错误。

V0.88(20140106)
1,support CSD new name rule and CSD new factory rework process
2,修改reserve1，reserve2没有下发的问题

V0.87(20131213)
1,修改multi页面里设定一个或以上NO USE (device = 0)， UI停不下来的问题
2,修改COtherStationDlg postrun fail会显示fail的问题;post run fail UI不做任何处理，以post Run之前的测试结果为准。

V0.86(20131205)
1,解决multi-UI引发的系列问题

V0.84(20131128)
1,解决LTE,CAMERA,2G3GTEST station默认为multi的问题。

V0.83(20131127)
1,解决V0.82 IMEI站传输机制漏洞,修改为:
  a.测试结果fail，如果IMEIUSED为Y，插入fail记录，否则不插入记录。
  b.测试pass，如果IMEIUSED不为Y，则修改errorcode为"IMEI0012"，插入fail记录。
  c.测试pass，如果IMEIUSED为Y，则上传pass记录
2,MMI_BB的重测机制恢复用新的rule，如果fail后，2次PASS 才算PASS
3,修改读db xml机制，原本是按Ln中的n递增读下去，导致如果n不连续，则后面的无法获取。修改方式，不去管n是否递增，只要这个节点存在即获取其值。

V0.82(20131126)
1,只有在T-Line第一站才会去强制写FA，原本的判断为第一站就会强制写，考虑到CSD tool，第一站也不需要写FA
2,应工厂要求，修改IMEI站传数据机制
    a, Fail在写IMEI之前，不插数据
    b, Fail在写IMEI 之后，IMEIUSED 为N, 不插数据。
    c, Fail在写IMEI 之后，IMEIUSED 为Y, 插一笔fail数据。
    d, PASS ，IMEIUSED 为N, 插一笔IMEI0012的fail数据。
    e, PASS ，IMEIUSED 为Y, 插一笔PASS数据
3,MMI_BB的重测机制也修改到旧的rule，如果fail后，1次PASS 就算PASS

V0.81(20131022-20131113)
1,multi-UI support STATION_2G3GTEST,STATION_LTE,STATION_CAMERA 用[station]下Multi=0或1来区分是否multi
2,升级ErrCodeGenerator.DLL为V1.006，修正了在路径有特殊字符“（”，“）”以及“ ”的时候check MD5 fail的情况
3,修正CR工单so count为0的情况，改为始终从so_info表里查询
4,Support 6K pn for T-Line, 9B/5J pn for P-Line

V0.80(20130822)
1,os_dl auto run DB信息未下发导致check routing fail,在check routing前增加了SetidParameter()
2,针对非dl站别的T-Line第一站，调整set picasso在pre-run之前方便记录log

V0.79(20130813)
1,不显示对话框

V0.78(20130812)
1,增加 multi-camera for boston
2,解决DownloadDlg.cpp 下发picasso值给dll的错误字符截断问题

V0.77(20130723)
1,Normal工单，FA/PA站，factory image list 更改为取VRCM和image_version_control表里release版的交集
2,删除UI上显示的HW Ver，也不会下发这个值给dll。UIControl.ini里的COMMON->HW_VER保留，暂时没有用到。RD用到的HWID之类信息请放在对应料号表里，RD版tool对应信息放在料号.ini里
3,料号.ini里的信息会下发当前station名称的section里的内容给dll（原本为0.76条目1内容）
4,从料号表或者SQN表获取的SW_version在map里取消，不会下发到dll中，以SO_info表query的SW_version为准

V0.76(0076)(20130626)
1,料号.ini里的信息修改为只有IMEI,FCHECK才会下发给dll，并且只下发当前station名称的section里的内容
2,解决VRCM查询语句bug
3,解决os_dl站load dll的问题
4,FA/PA normal工单，如果获取的image多于一个，则不获取image_version_control里的reserve1和reserve2的值，下发为空

V0.74(0074)（20130617）Nancy.Yang
1,Normal工单，FA/PA站，factory image list 更改为从VRCM获取，原本是从image_version_control表里获取
2.VMS_DBACESS.DLL 更新由到1.0.1.8,增加接口 VMS_QueryVRCMRImageList

V0.73(0073)（20130520）Nancy.Yang
1,增加 LTE 站别，使用dll为 RF.dll

V0.72(0072)（20130510）Nancy.Yang
1,修改check routing流程，针对CSD与Normal机种转换后在CIM里检查规则
2,在DBAcess CheckSO中增加了对 D 状态的判断,D表示DOA/RMA机种
3,须搭配 DBAcess V1.0.7.7版本或以上使用

V0.71(0071)
1.A12這個Project的Camera測試有三站，增加Unify UI的站別分別為 CAMERA1, CAMERA2（原本的CAMERA不要動到）。
V0.70(0070)
1.UI实现过下发image_version_control 里的Reserve1, Reserve2的值
2.VMS_DBACESS.DLL 更新由到1.0.1.7
V0.69(0069)
1.Move POP弹出对话框去左上角

V0.68(0068)
1.修改ACER22 编辑框名为 Serial No
2.扫入IMEI后，光标自动跳转到Serial No栏位

V0.67(0067)
1.增加新的EDIT框用于输入ACER22码
2.在配置档UIControl.ini中加入ACER22项配置项， 当ACER22=1时，ACER22可显示操作，原理同NAL相似
3.当ACER22输入且符合rule，SetParameterValue("ACER22",sz_Acer22);关键字为"ACER22" 

V0.66(0066)
1.修正IMEI/FCHECK站，当配置档中NAL = 1 时，获取UI界面的NAL框值，如果Query得到的NAL为空，则将扫入的NAL编辑框内值设置参数到DLL中
  
V0.65(0065)
1.配合SO control增加的fix option，当fix为‘Y’时，固定使用工单指定的image来生产；其它情况按照V0.64所述考虑。需配合使用最新版DB(1.0.7.4)
2.调整UI点击GO之后Power Supply和Device的启动顺序为power OFF PS-> device IN-> power ON to Valtage1-> power ON to Valtage2.
3.更新error code

V0.64(0064)
1.修正0.62版之问题，在FA及PACKAGE线，只有非CR工单查询PVCS得到的image version list，CR工单以CIM中维护image version为准。

V0.63(0063)
1.修正FCHECK站，对于同一张工单中重做的手机做OOB check的误判，使其不做OOB check。

V0.62(0062)
1.在FA及PACKAGE线，只有Normal工单查询PVCS得到的image version list，其余情况以SO中维护image version为准。需使用VMS DB 1015以上版本。

V0.61(0061)
1.增加判断，当UI启动时，如果有Power Supply控制，就把Power Supply关掉

V0.60(0060)
1.修正checkrouting 失败时不运行postrun的问题
2.HWVersion如果在PVCS中有定义则显示PVCS中定义的部分
3.修改RD版head图片为灰白色与工厂版区别
4.增加部分log
5.增加Re-DL站
6.DB插入CR信息接口更新，需要使用最新DB测试

V0.59(0059)
1.修改CSD工单首站只查Tline段
2.CSDPREMMI站使用MMIBB.dll
3.修正NAl_SCAN站未下发picasso的bug
4.修改QAhold显示信息便于PE解除

V0.58(0058)
1.修改启用PVCS后imagelist下发的查询流程，CR工单只下发V状态的imagelist，如果没有下发R状态的，Normal工单只下发R状态的image

V0.57(0057)
1.PVCS是否启用的控制开关SQN不再放在UIcontrol配置文件中移至当前目录Qisda下config.ini文档中，FAArea与NewErrcode一并移至此处
2.修复DL站输入大写D无法输入的BUG
3.比对IMEI与picasso对应关系失败时记录picasso号码到log中
3.修改CSD工单检测前站的流程
4.修正由于插入数据与检测重测接口调用顺序导致的重测一次不显示的bug

V0.56(0056)(20120717)
1.修改errorcode机制，不上传部分errorcode可以通过csv文档确定
2.bug fixed:修正一次retest之后，errormsg变大导致显示不全的bug
3.bug fixed:修正fcheck站，如果有送检OOB，会将errorcode覆盖不显示的bug
4.bug fixed:修正料号为CS开头时不查询PVCS数据的bug
5.修正DB1024显示信息为请去TS解锁
6.更新UIerrorcode为新errorcode
7.RD版中由于某些机种第一站为BBTEST，需要写入picasso，故RD可将UIControl中该站后加_01表示第一站，会写入picasso
8.调整IMEI&Picasso对应关系检查到checkrouting之前，避免已经PASS的IMEI号被其他手机做Fail的问题。
9.DB及UI的errorcode都不会上传CIM
10.IMEI page增加下发HW_VERSION

V0.55(0055)(20120703)
1.扫描picasso的站点，在未读出picasso之前的错误依然记入CIM
2.提醒不显示pass，只显示提醒重做一次
3.减小errormsg字体避免过长时只显示部分信息
4.DB的errorcode不传CIM
5.NewerrCode默认为1
6.被HOLD住的状态为H时不在用checkroutingfail表示，用UI1021表示
7.员工号隐藏
8.加深各个版本颜色，修改show result框颜色，黄色Retest，红色Fail，绿色Pass
9.添加BIST站，使用downloadPage，DL.dll或BIST.dll
10.所有工单在FA和package均下发经过验证的factoryversionlist
11.增加NAL_SCAN station
12.拿掉check NAL 前6位

V0.54(0054)
1.修正bug：download page页面事件未注册
2.修正BB为第一站时由于rework标签未复位导致的UI1103错误


V0.53(0053)20120611
1.增加log管道到4096避免log数据超长而导致记录信息丢失
2.Fcheck站CheckOOB的sample rate计算方式修改
3.UI退出是增加kill ADB的动作，避免entrance tool无法删除文件
4.RD版不卡手机中是否有picasso
5.Powersupply开启控制改为0->powerVoltage1->powerVoltage2,关闭为powerVoltage1->0，为保证与以前版本兼容，配置文档的参数不做修改，以关闭电压作为powerVoltage1
6.CR并启用SQN时，factoryversion从VMS中抓取目前所有有效的版本构成image列表，测试版本为其中一个即可
7.新增站名dll命名规则修订如下：otherpage station目前已有的站名对应dll名称不变，新增站默认使用FT.dll,如没有,使用"站名.dll",其他page页如果加载不成功也使用"站名.dll"

V0.52(0052)
1.download page增加relayboard控制，同时支持其他dll通过发送relayboard控制消息来控制relayboard
2.otherstation page支持powersupply及relayboard的控制，dll端如要使用需要注册这两个消息
3.Uicontrol中各station增加是否使用relayboard，及relayboard number，是否反序的控制

V0.51(0051)
1.bugfixed：修正DL&trigger站使用新的errorcode机制时提示新errorcode没有找到的bug及其多个slot失败时第一个以后的errorcode沿用第一个的bug

V0.50(0050)
1.增加Trigger&DL站FA写入后读出比对，避免未正确写入
2.精简powersupply控制类
3.执行杀死msswchx的时如果超时导致的错误不再提示，记录到log中避免出现没有该进程导致报错


V0.49(0049)
1.bug fixed:测试失败时errorcode赋值导致无法插入失败数据
2.bug fixed:不启用新errorcode机制时errormessage不显示的问题
3.bug fixed;控制多个powersupply时只有一个起作用的问题
4.bug fixed:修复不提示重测一遍的bug

V0.48(0048)
1.bug fixed:修正V0.47版trigger站RD版始终要求输入软体版本的bug以及删除配置文档更换model后提示找不到配置文件无法输入的bug
2.增加多powersupply控制，控制顺序为按序号顺序打开，可以在各站名称下设置是否需要powersupply及需要几个
3.FCheck站的送检OOB检测移到UI，如果需要送检OOB，则以“送检OOB”代替“PASS”
4.bug fixed:插入CIM数据时不再以dll是否有设置id的值为标准，直接以IMEI站以IMEI为id，其他站以picasso为id
5.增加4GPTEST站，对应RF.dll
6.RD版增加DL为第一站,执行写入初始化FA动作
7.OSDL读物picasso全部失败时不再继续download
8.增加新errorCode机制可以通过Uicontrol文档中common下NewErrCode=1来启用
9.显示errorcode的中文解释在result框中。

V0.47
1.bug fixed:修正由于UI配置文件不匹配导致的时间同步报错为fastboot time out的bug
2.bug fixed:修改RD版initial dialog输入的软体版本号为使用版本号，release版依然以SQN开关作为是否使用uiconfig文档的版本号作为基准。
3.bug fixed:修改trigger与osdl站分别根据factoryversion与userload version来查找imagepath
4.对code中未维护的新增站不再报错，而是使用默认FT.dll，otherdlg的UI显示页
5.RD版本起始站名选择框的内容从UI配置文件中读取，新加站只需在uiconfig文档中增加，不在code中维护
6.增加对员工号的控制，EntranceTool首先锁定当前员工号，退出UnifyUI时解锁当前员工号，需要DBAccess.dll版本V1.052及以上

V0.46
1.由于toolversion从CIM抓取，增加对ToolVersion的检测，对应errorcode UI1205
2.增加BOOT站

V0.45
1.修改QueryHoldinfo的DB接口为V2版本
2.增加对IMEI站NAL合法性的检测
3.需要DBaccess版本1.048及以上

V0.44
1.UI1006-CR工单数量超限不存入PICS
2.MAX_BUFFER_SIZE重定义为4096字节大小
3.修正CR/BO工单没有插入CRcotrolinfor的bug
4.增加与server 时间同步的功能，server IP与同步时间间隔可以在UI配置文档中修改

V0.43
 ---2012.02.22新增(1.008)
1.增加CSD工单的卡关机制
2.增加BO，CR工单的卡关机制
3.增加对DB的DB1024,DB1025的Errorcode不上传CIM
4.需要DBAccess.dll（V1.044）及以上版本配合使用
-----(1.007),2012.02.09
1.将cycletime写入pics,使用pics文件的L2列，注意需要在pics文档中将L2列设置为cycletime
2.bugfixed：修正用于统计测试结果的本地记录文件在调试时提示路径无法找到的bug
3.修改UI获取toolversion方式，原先为从CIM中抓取，抓不到则显示UI定义的version，现改为，从dll中获取version，关键字为"TOOL_VERSION",如果获取不到则显示UI本身的version如V0.43,getparametervalue 位于begin之后


V0.42(1.006),2012.02.04
1.整理UI为三个编译模式，RD_MODE，TS_MODE，FACTORY_MODE，其中TS_MODE由原先prerelease模式改变而成，去掉checkrouting，
	测试完成后不插入测试记录（OSDL站会在MMIBB插入TS pass记录，用于rework控制）;FACTORY_MODE下输入正确的工单号，TS的
	员工号也可以进入TS模式，与单独的TS模式相同。
2.powersupply打开之前增加延迟，延迟时间与打开之后一致。
3.应A9要求，增加CURRENT站，ONLINE_WLS3站
4.修改UI标题的显示为根据站名绘制标题，不再使用站名图片，统一使用header.jpg,免去每次加站需要PS图片之苦。


V0.41(V1.005),2012.01.17
1.bug fixed: IMEI站检测picasso与IMEI有不匹配时，如postrun上传pass记录会覆盖原先UI1052而显示pass的bug
2.bug fixed: 修复pics数据没有清除的bug，避免失败手机的未产生的数据复制了前一个手机
3.IMEI站增加对IMEI status及picasso对应的CIM是否唯一的检测
4.IM1200-IM1204，UI1052，UI1055的errorcode不写入pics数据

V0.40(V1.004),2012.01.16
1.IMEI站DB table增加5列，picasso，sw，hw, lineid,mal分别对应l1,l9,l8,line,l13
2.每个手机测试开始时增加是否hold检测，如果被QAhold则不允许继续测试(UI1201)，imei和fcheck会做picasso和imei的双重hold检测,需要DBacess.dll(V1.038及以上版本的配合)
3.UI的配置文档UIconfig放置于C:\\PE_Config\\{model name}\\Setting目录下
4.只卡CR工单的数量
5.为避免UI显示版本被修改，UI显示的toolversion不再支持在ini文档中配置，因此toolversion目前两种显示可能：
	(1).从CIM中获取到了toolversion；
	(2).CIM中为空，默认使用UI定义的toolversion，修改UI定义的toolversion请修改commdef.h中VERSION_INFO的宏定义。
6.料号文件放置于C:\\PE_Config\\{model name}\\PartNumber目录下
7.download page 添加下发MODEL_NAME
8.IMEI空的key value不显示在UI上
9.FDCS不启用时，Factory version，SW version从UIconfig文件中读取
10.修正convertstring函数入参为空时可能导致的crash的bug


V0.39,2012.01.02
1.修改UI记录log及测试结果时如果不关闭Tool不能按日期新建的bug,log放到D:\\Log\\UI下面与DB的log路径一致
2.导入FDCS，RD版从本地9J料号配置文件中获取（与UI的配置文件都放在C:\PE_Config\目录下），通过配置文件中SQN可配置是否启用该功能,如需要启动该功能请使用VMS_DBAccess.dll（1.0.0.4）及DBAccess.dll（1.0.3.5）或更高版本，如果不启用将从默认的配置文件中读取
3.将V0.37中增加的对Userload与料号对应关系的检测移到DB中检测
4.UI配置文件名字去掉modelname方便各project通用,配置文件位置移到C:\PE_Config\目录下，由PE控制
5.About对话框中增加版本信息
6.去掉SOCNT可配置，去掉未使用的initialFA，默认不写FA结果
7.采用AJ chang建议如果有治具控制，在每次治具控制开始前杀死msswchx.exe
8.根据Chris提示增加otherstation和IMEI两个page的checkpicasso失败时执行postrun
9.由于T/A line使用factory version，pline使用userload version，增加setparametevalue，关键字：“FACTORY_VERSION”，对应TA两线的软体版本
10.增加对dll下发log文件的保存路径使用关键字：“UI_LOG_PATH”, UI的log路径为release版D:\\Log\\UI，RD版.\\UI
11.增加对dll下发RD或者release版的关键字：“TOOL_MODE”，值为：RD版为“RD”，release版为“FACTORY”
12.针对download站下发key--"DLMODE",value--"PreDL"或"ReDL"
13.修正IMEIpage没有下发SW和Factory version的bug

V0.38
1.微调NAL机制，IMEI站用于NAL扫入，FCHECK站查询出NAL显示在UI上由OP用于重复比对
2.控制powersupply关闭时关闭电压由配置文件读取，满足各project对关闭电压的要求
3.在checkrouting中增加对是否超过工单额定数量的判断,是否启用通过配置文件中SOCNT控制，1表示启用，0表示不启用
4.IMEI站扫入IMEI及NAL后即做是否满足长度的检测，如果不满足长度重新设置为空，如果满足长度但OP扫入颠倒，自动更正
5.UIlog记录每条时添加时间
6.OS_DL流程优化，可以根据UI配置文件配置是否读写FA
7.一次失败需要成功两次才能通过，在第一次成功时UI提示再做一次
8.bug fixed:使用旧FA时mobiled可能会变为乱码插入picassocontrol表导致后续无法继续
9.stdafx去#include"afxcontrolbars.h"改为#define CWinAppEx CWinApp减小可执行文件大小到800k附近

V0.37
1.将UI的error也写入CIM中，注意只有在picasso输入框有显示的picasso或者IMEI（包括扫描和从手机中读取）的才能被写入
2.修正IMEI站手机中IMEI&Picasso对应关系与CIM中不匹配是错误信息中显示乱码的问题。
3.添加各站是否检测userload(SW)及料号对应合法性的功能，用于OSDL及其以后station，是否启用通过配置文件中各站下SQN配置，
  为1表示启用，0不启用，默认为不启用，启用时需要与VMS_DBAccess.dll(V1.0.0.2)及以上配合，同时在DB的ini配置文档中需要对应的配置文件
4.增加配置文件中NAL_6，用于各project要比较的NAL前六位，仅在NAL有效时有效


V0.36
1.测试处理流程代码优化
2.修改日志保存目录，RD和prerelease版本保存在当前目录.\\UI，release版保存在D:\\TestLog\\UI下，以便于后续entrancetool联合使用
3.IMEI站增加入网许可证输入框，可通过配置文档配置是否显示NAL=0不显示，NAL=1显示
4.非RD模式下增加对IMEI和NAL对应关系匹配检测，需要DBAccess.dll版本V1.0.2.8及以上支持
5.去掉配置文件中project项，改完在每站中添加VKEnter来配置该站是否对回车键响应，1表示响应，0表示不响应，默认为响应
6.对测试过程中弹出对话框，使用新的dialog，不再使用原先afxmessagebox，以便增大显示字体，及后续增加可能的其他功能配置


V0.35
1.增加dll共用begin(),end()接口需要dll端共同实现
2.修改prerun()的逻辑，改为每次点击GO运行一次，begin()接口用于tool开启时dll端一些变量的初始化
3.修改不读FA时写FA无效，即要写FA必须先读
4.在配置文件中增加项目Project，1表示wolf，2表示orange，3表示M66......otherstationdlg.cpp中的对回车符的处理中wolf默认不响应回车符。
5.其他一些逻辑判断的细节修改


V0.34
1.checkrouting规则改完失败一次需要成功两次才能进行下一站
2.增加读写1MFA区域的接口GetFASector()SetFASector，可以通过配置文件中FAArea配置1使用1M的FA区域，0使用512的字节区域
3.添加audio站，dl站
4.设置从数据库中获取factoryversion,userversion,toolversion为空时，从配置文件中读取这三项，UI的版本号可以通过TOOL_VER=xxx自行配置
5.与entrance Tool配合使用时UI接收5个参数，其中最后一个为imagepath，如果为-，由UI自行组织，此项对dl站起作用


V0.33
1. 增加2G3GTest（T）和ALS（P）两站
2. 去掉relayboard控制部分及错误代码及相应的ini文件读取部分
3. 控制powersupply把关闭动作改为将电压设为2V，避免电压冲击
4. 控制powersupply接口，地址可以从配置文件读取

V0.32
1.IMEI站增加对CDMA的IMEI算法的验证
2.更改UI的log记录方式改为管道避免多次打开文件导致的日志丢失及可能的crash问题
3.控制治具方面增加打开串口、发送数据失败返回错误代码
4.otherstation页增加备选输入框，可以在当前手机正在测试时在备用输入框中扫入picasso，等当前测试完毕实现自动切换到下一个，此项可配置

V0.31
1.修改配置文件部分配置参数，添加对relayboard的控制
2.修正download站输入picasso相同检测不出的问题，download将检测picasso相同移到主UI实现统一处理
3.去掉未使用的errorcode，修改部分errorcode，使各站相同操作显示的errorcode相同
4.download站对读写FA加锁，避免底层dll出现crash
5.对Faoperator类做了较大修改，去掉未使用的函数，修改报错方式
6.对手机中是否有picasso做了统一的设定以手机中存在合法的picasso作为存在标准，避免可能存在的乱码影响
7.UI显示将原PCBA改为SW Ver，将User load Ver改为SW Ver
8.每次GO清空errorcode和errormsg避免上次的errorcode显示
9.针对dll可能返回空errorcode的问题，对收到空errorcode时提示UI0000及未知dll错误的提示信息
10.trigger站和osdl站writeFA可配置
11.修正osdl读取FA失败后返回界面时errorcode被清除的问题
12.Aline添加MMI_TEST站，使用otherstation的page

V0.30
1.添加配置文件，可以控制powersupply开关，治具需要治具控制及自定义控制命令，是否需要扫描枪
2.将开始获取picasso放入进程中，避免点GO后出现短时间的界面无响应
3.OS_DL修改流程
4.全局函数加锁避免出现多线程调用时出现错乱
5.将串口控制单独拿出放到通讯类中
-----10.10
6.修改ontimer内用clock()函数计算秒数引起时间不走的问题
7.将osdl和trigger的线程函数合并
8.修正osdl获取picasso时第一个获取失败导致界面返回的问题
9.打开powersupply后根据配置文件的等待时间sleep，等串口出现
10.判定为rework的条件变更为picasso完全合法，mobileid不是全部相同的数字，rd版目前不写mobileid

V0.29
1.Aline Tline前两站添加扫入picasso与手机中picasso比对，添加两个errorcode，用于表示未扫描及扫入的picasso与手机中不匹
配
2.修正entrancetool传入参数时解析toolver和swversion错序问题
3.修正UIerror时show结果时计时器未停止的问题
4.修正写FAresult，没有stationindex时可能溢出崩溃的bug
5.通过setparametervalue传递HW_VERSION SW_VERSION给dll用于验证软硬件版本号，trigger和os_dl除外
6.调整治具控制命令
7.调整控制治具命令下发在show result之前
8.修正trigger站所有picasso未输入时不提示错误的bug
9.其他细节调整

V0.28
1.download将错误代码细化，区分UI1033和UI1032错误，将会由具体的DB错误代码代替。
2.修正FA写不进去的bug
3.去掉IMEI站检查picasso和mobileid的对比
4.修正查询IMEI号对应的PTIMEI记录不存在，返回false时认为查询失败的bug
5.添加部分log便于排错

V0.27
1.修正RD版如果手机内picasso长度不足会报错的bug。
2.修正IMEI站test fail和write FA都发生时显示后一个errorcode的问题。
3.增加测试记录文件，记录time，picasso，Errorcode，ErrorMsg存在本地log目录下以“站名_日期.csv”命名，每天保存一个，用于统计信息。
4.修改IMEI站RD版不写FA结果，目前RD版所有站的FA读写情况为：trigger和os_dl不读也不写FA，其他站读取FA但不写入测试结果。
5.修改部分log记录便于阅读。
6.修正FFU站未维护导致的UI1015错误


================================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : UnifyUI_FacTestTool Project Overview
===============================================================================

The application wizard has created this UnifyUI_FacTestTool application for
you.  This application not only demonstrates the basics of using the Microsoft
Foundation Classes but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your UnifyUI_FacTestTool application.

UnifyUI_FacTestTool.vcproj
    This is the main project file for VC++ projects generated using an application wizard.
    It contains information about the version of Visual C++ that generated the file, and
    information about the platforms, configurations, and project features selected with the
    application wizard.

UnifyUI_FacTestTool.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CUnifyUI_FacTestToolApp application class.

UnifyUI_FacTestTool.cpp
    This is the main application source file that contains the application
    class CUnifyUI_FacTestToolApp.

UnifyUI_FacTestTool.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
    Visual C++. Your project resources are in 1033.

res\UnifyUI_FacTestTool.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file UnifyUI_FacTestTool.rc.

res\UnifyUI_FacTestTool.rc2
    This file contains resources that are not edited by Microsoft
    Visual C++. You should place all resources not editable by
    the resource editor in this file.


/////////////////////////////////////////////////////////////////////////////

The application wizard creates one dialog class:

UnifyUI_FacTestToolDlg.h, UnifyUI_FacTestToolDlg.cpp - the dialog
    These files contain your CUnifyUI_FacTestToolDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's template is
    in UnifyUI_FacTestTool.rc, which can be edited in Microsoft Visual C++.


/////////////////////////////////////////////////////////////////////////////

Other Features:

ActiveX Controls
    The application includes support to use ActiveX controls.

/////////////////////////////////////////////////////////////////////////////

Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named UnifyUI_FacTestTool.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

UnifyUI_FacTestTool.manifest
	Application manifest files are used by Windows XP to describe an applications
	dependency on specific versions of Side-by-Side assemblies. The loader uses this
	information to load the appropriate assembly from the assembly cache or private
	from the application. The Application manifest  maybe included for redistribution
	as an external .manifest file that is installed in the same folder as the application
	executable or it may be included in the executable in the form of a resource.
/////////////////////////////////////////////////////////////////////////////

Other notes:

The application wizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, you will need
to redistribute the MFC DLLs. If your application is in a language
other than the operating system's locale, you will also have to
redistribute the corresponding localized resources MFC90XXX.DLL.
For more information on both of these topics, please see the section on
redistributing Visual C++ applications in MSDN documentation.

/////////////////////////////////////////////////////////////////////////////
