V1.084(20140715)
1,IQT 站改为抽检，GetProcessResult 里修改 PTIQT没有记录也return true。
2,mark CloseDB->Clear_PICSData() for QA tool can't find xml file.

V1.083(20140520)Magan.Mei
add function 'Query_IDType' and 'QueryApkTestItem_V2'

V1.082(20140310)
fix crash issue when so type is 'D'.

V1.081(20140106)Magan.Mei
1.new csd name rule and new csd factory process rule

V1.080(20131126)Magan.Mei
1. MMI_BB的重测机制新rule，如果fail后，2次PASS才算PASS
2. 修改读db xml机制，原本是按Ln中的n递增读下去，导致如果n不连续，则后面的无法获取。修改方式，不去管n是否递增，只要这个节点存在即获取其值。

V1.079(20131126)Magan.Mei
1. V1.079 is only for Dublin-3G MMI_BB的重测机制也修改到旧的rule，如果fail后，1次PASS 就算PASS

V1.078 (20131113) Magan.Mei
1. Add QueryApkTestItem_V2 add switch flag

V1.077（20130510）Nancy.Yang
1，增加 CheckRouting_V2 用于导入新的 CSD 与 normal 机种切换的流程
