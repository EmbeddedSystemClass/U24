V2.013(20120520) Nancy.Yang 
1,增加 SO D 类型的判断

V2.012（2012）（备注：还未启用VRCM 20120520）
1.新增PN（料号），image（Factory/User）,Tool Version三者记录查询，在VRCM表中

2.详细逻辑
a)在Entrance Tool中增加判断PN+ Factory Image（OS Image）Version + Tool version是否匹配的判断
b)根据SO中的PN/Factory （OS）Image Version/Tool version 在VRCM table中查询对应的记录
1）无Model记录，不做判断。
2）有记录，根据PN查询，并且最新的纪录status = 1 ,Record_status=2可以生产
3）有记录，根据PN查询，但是最新纪录Status和Record_status状态不正确，不能生产
4）5E/5G使用Factory Image version， 9J/CS使用OS Image version

3.VMS_DBACESS.DLL版本由1.015升级到1.016


V2.011（2011）
1.增加Fix_Entrance_Ver判断，当Fix为 ‘Y’时，判断当前版本和Fix指定版本是否一致，不一致，则自动升级为Fix指定版本
 
2.当Fix为 'N'时，判断当前版本同最新的release版本是否一致，不一致，则自动升级为最新release版本
 
3.setup.ini 删除一些option

4.MD5 CHECK FAIL

================================================================================
MICROSOFT 基础类库: Entrance Tool 项目概述
===============================================================================

应用程序向导已为您创建了这个 Entrance Tool 应用程序。此应用程序不仅演示 Microsoft 基础类的基本使用方法，还可作为您编写应用程序的起点。

本文件概要介绍组成 Entrance Tool 应用程序的每个文件的内容。

Entrance Tool.vcproj
这是使用应用程序向导生成的 VC++ 项目的主项目文件。 
它包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

Entrance Tool.h
这是应用程序的主要头文件。它包括其他项目特定的头文件(包括 Resource.h)，并声明 CEntranceToolApp 应用程序类。

Entrance Tool.cpp
这是包含应用程序类 CEntranceToolApp 的主要应用程序源文件。

Entrance Tool.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。此文件可以直接在 Microsoft Visual C++ 中进行编辑。项目资源位于 2052 中。

res\Entrance Tool.ico
这是用作应用程序图标的图标文件。此图标包括在主要资源文件 Entrance Tool.rc 中。

res\EntranceTool.rc2
此文件包含不在 Microsoft Visual C++ 中进行编辑的资源。您应该将不可由资源编辑器编辑的所有资源放在此文件中。


/////////////////////////////////////////////////////////////////////////////

应用程序向导创建一个对话框类:

Entrance ToolDlg.h，Entrance ToolDlg.cpp - 对话框
这些文件包含 CEntranceToolDlg 类。该类定义应用程序主对话框的行为。该对话框的模板位于 Entrance Tool.rc 中，该文件可以在 Microsoft Visual C++ 中进行编辑。


/////////////////////////////////////////////////////////////////////////////

其他功能:

ActiveX 控件
应用程序包括对使用 ActiveX 控件的支持。

/////////////////////////////////////////////////////////////////////////////

其他标准文件:

StdAfx.h，StdAfx.cpp
这些文件用于生成名为 Entrance Tool.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

Resource.h
这是标准头文件，它定义新的资源 ID。
Microsoft Visual C++ 读取并更新此文件。

Entrance Tool.manifest
	应用程序清单文件供 Windows XP 用来描述应用程序
	对特定版本并行程序集的依赖性。加载程序使用此
	信息从程序集缓存加载适当的程序集或
	从应用程序加载私有信息。应用程序清单可能为了重新分发而作为
	与应用程序可执行文件安装在相同文件夹中的外部 .manifest 文件包括，
	也可能以资源的形式包括在该可执行文件中。 
/////////////////////////////////////////////////////////////////////////////

其他注释:

应用程序向导使用“TODO:”指示应添加或自定义的源代码部分。

如果应用程序在共享的 DLL 中使用 MFC，则需要重新发布这些 MFC DLL；如果应用程序所用的语言与操作系统的当前区域设置不同，则还需要重新发布对应的本地化资源 MFC90XXX.DLL。有关这两个主题的更多信息，请参见 MSDN 文档中有关 Redistributing Visual C++ applications (重新发布 Visual C++ 应用程序)的章节。

/////////////////////////////////////////////////////////////////////////////
