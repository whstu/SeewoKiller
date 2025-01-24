<!--markdownlint-disable MD001 MD033 MD041 MD051-->

<div align="center">

# <image src="ClassIsland/Assets/AppLogo_AppLogo.svg" height="28"/> ClassIsland

<!-- ![软件截图 - 主界面](https://github.com/ClassIsland/ClassIsland/assets/55006226/65e2bdba-be83-444c-b42f-b893aaace9c3) -->

# ![Banner](https://github.com/user-attachments/assets/a815dd7d-8343-4da5-aee4-3f754aa297e4)

[![Stars](https://img.shields.io/github/stars/whstu/SeewoKiller?label=Stars)](https://github.com/whstu/SeewoKiller)
[![正式版 Release](https://img.shields.io/github/v/release/whstu/SeewoKiller?style=flat-square&color=%233fb950&label=正式版)](https://github.com/whstu/SeewoKiller/releases/latest)
[![测试版 Release](https://img.shields.io/github/v/release/whstu/SeewoKiller?include_prereleases&style=flat-square&label=测试版)](https://github.com/whstu/SeewoKiller/releases/)
[![下载量](https://img.shields.io/github/downloads/whstu/SeewoKiller/total?style=social&label=下载量&logo=github)](https://github.com/whstu/SeewoKiller/releases/latest)

希沃克星：冰点解冻、游戏和晚自习制裁！<br/>
使用善良的GUI而不是进Windows PE

#### 💬[Classlsland QQ 群组](https://qm.qq.com/q/6ZAbcZSK5y)

#### [🌐 官方网站](https://whstu.us.kg/)｜[📚 项目文档](https://github.com/whstu/SeewoKiller/wiki)

</div>

## 开始使用

**首先，请确保您的设备满足以下推荐需求：**

- Windows 7 及以上版本的系统

> [!IMPORTANT]
> **详细安装说明请参阅 [ClassIsland 文档](https://docs.classisland.tech/app/setup)。**
>
> 不建议在 Windows 10 以下的系统运行本应用。在 Windows 7 中，.NET 运行时会产生**严重的内存泄漏问题**。如果您执意要在 Windows 7 中使用 ClassIsland，请参阅 [在 Windows 7 中安装 ClassIsland](https://docs.classisland.tech/app/setup#检查系统需求)。

对于普通用户，可以在以下渠道下载到本软件，请根据自身网络环境选择合适的渠道。

> 测试版包含最新的功能，但也可能包含未完善和不稳定的功能。

| 下载渠道  | **🚀 正式版**<br/>[![正式版](https://img.shields.io/github/v/release/ClassIsland/ClassIsland?style=flat-square&color=%233fb950&label=)](https://github.com/ClassIsland/ClassIsland/releases/latest) | 🚧 测试版<br/>[![测试版](https://img.shields.io/github/v/release/ClassIsland/ClassIsland?include_prereleases&style=flat-square&label=)](https://github.com/ClassIsland/ClassIsland/releases/) |
| :-------: | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: | :-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------: |
|  GitHub   |                                                            [**下载**](https://github.com/ClassIsland/ClassIsland/releases/latest)                                                            |                                                              [下载](https://github.com/ClassIsland/ClassIsland/releases)                                                               |
| AppCenter |                                    [**下载**](https://install.appcenter.ms/users/hellowrc/apps/classisland/distribution_groups/public/releases/latest)                                    |                                 [下载](https://install.appcenter.ms/users/hellowrc/apps/classisland/distribution_groups/publicbeta/releases/latest)                                 |

如果以上链接无法使用，可以从 [镜像链接](https://docs.classisland.tech/app/setup#%E4%B8%8B%E8%BD%BD%E5%BA%94%E7%94%A8%E6%9C%AC%E4%BD%93) 下载。

下载完成后，将软件压缩包解压到一个**独立的文件夹（运行路径不能有中文）**，运行软件即可开始使用。解压时请不要解压到网盘同步文件夹、【下载】文件夹中，否则可能会出现**文件无法读写、文件丢失**等问题。

## 获取帮助＆加入社区

您可以访问以下页面来**获取帮助**：

- [ClassIsland 帮助文档](https://docs.classisland.tech/app)

您也可以加入这些社区**寻求帮助**：

[![GitHub Issues](https://img.shields.io/github/issues-search/ClassIsland/ClassIsland?query=is%3Aopen&style=flat-square&logo=github&label=Issues&color=%233fb950)](https://github.com/ClassIsland/ClassIsland/issues)
[![GitHub Discussions](https://img.shields.io/github/discussions/ClassIsland/ClassIsland?style=flat-square&logo=Github&label=Discussions)](https://github.com/ClassIsland/ClassIsland/discussions)
[![加入 QQ 频道](https://img.shields.io/badge/QQ_%E9%A2%91%E9%81%93-classisland-%230066cc?style=flat-square&logo=TencentQQ)](https://pd.qq.com/s/scb3wzia)
[![加入 QQ 群](https://img.shields.io/badge/QQ_%E7%BE%A4-958840932-%230066cc?style=flat-square&logo=TencentQQ)](https://qm.qq.com/q/4NsDQKiAuQ)

如果您确定您遇到的问题是一个 **Bug**，或者您要提出一项**新的功能**，请[提交 Issue](https://github.com/ClassIsland/ClassIsland/issues/new/choose)。

## 开发

本项目目前开发状态：

- 正在 [`dev`](https://github.com/ClassIsland/ClassIsland/tree/dev) 分支上开发版本 [1.6 - Himeko](https://github.com/ClassIsland/ClassIsland/milestone/7)。
- 正在 [`master`](https://github.com/ClassIsland/ClassIsland/tree/master) 分支上维护版本 [1.5 - Griseo](https://github.com/ClassIsland/ClassIsland/milestone/6)。

要在本地编译应用，您需要安装以下负载和工具：

- [.NET 8.0 SDK](https://dotnet.microsoft.com/zh-cn/download/dotnet/8.0)
- [Visual Studio](https://visualstudio.microsoft.com/)

对于 Visual Studio，您需要在安装时勾选以下工作负载：

- .NET 桌面开发

如果您有意愿为 ClassIsland 做出代码贡献，请先阅读 [贡献指南](CONTRIBUTING.md) 来了解如何为 ClassIsland 做代码贡献。我们欢迎想要为本应用实现新功能或进行改进的同学提交 [Pull Request](https://github.com/ClassIsland/ClassIsland/pulls)。

## 致谢

<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-18-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

本项目受到 [DuguSand/class_form](https://github.com/DuguSand/class_form) 的启发而开发。

感谢以下同学为本项目为本项目的开发提供支持（[✨](https://allcontributors.org/docs/zh-cn/emoji-key)）：

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/HelloWRC"><img src="https://avatars.githubusercontent.com/u/55006226?v=4?s=100" width="100px;" alt="HelloWRC"/><br /><sub><b>HelloWRC</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=HelloWRC" title="Code">💻</a> <a href="#design-HelloWRC" title="Design">🎨</a> <a href="https://github.com/ClassIsland/ClassIsland/commits?author=HelloWRC" title="Documentation">📖</a> <a href="#ideas-HelloWRC" title="Ideas, Planning, & Feedback">🤔</a> <a href="#maintenance-HelloWRC" title="Maintenance">🚧</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/Doctor-yoi"><img src="https://avatars.githubusercontent.com/u/106463935?v=4?s=100" width="100px;" alt="Doctor-yoi"/><br /><sub><b>Doctor-yoi</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=Doctor-yoi" title="Code">💻</a> <a href="#question-Doctor-yoi" title="Answering Questions">💬</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://www.jiangyin14.top/"><img src="https://avatars.githubusercontent.com/u/106649516?v=4?s=100" width="100px;" alt="姜胤"/><br /><sub><b>姜胤</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=jiangyin14" title="Documentation">📖</a> <a href="#ideas-jiangyin14" title="Ideas, Planning, & Feedback">🤔</a> <a href="https://github.com/ClassIsland/ClassIsland/issues?q=author%3Ajiangyin14" title="Bug reports">🐛</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://fossa.com/"><img src="https://avatars.githubusercontent.com/u/29791463?v=4?s=100" width="100px;" alt="fossabot"/><br /><sub><b>fossabot</b></sub></a><br /><a href="#infra-fossabot" title="Infrastructure (Hosting, Build-Tools, etc)">🚇</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://allcontributors.org/"><img src="https://avatars.githubusercontent.com/u/46410174?v=4?s=100" width="100px;" alt="All Contributors"/><br /><sub><b>All Contributors</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=all-contributors" title="Documentation">📖</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/TV-ZHU"><img src="https://avatars.githubusercontent.com/u/88492699?v=4?s=100" width="100px;" alt="DSZDev"/><br /><sub><b>DSZDev</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=TV-ZHU" title="Documentation">📖</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/LiuYan-xwx"><img src="https://avatars.githubusercontent.com/u/66517348?v=4?s=100" width="100px;" alt="流焰xwx"/><br /><sub><b>流焰xwx</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=LiuYan-xwx" title="Documentation">📖</a> <a href="https://github.com/ClassIsland/ClassIsland/commits?author=LiuYan-xwx" title="Code">💻</a></td>
    </tr>
    <tr>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/TuanZiGit"><img src="https://avatars.githubusercontent.com/u/46892455?v=4?s=100" width="100px;" alt="团子"/><br /><sub><b>团子</b></sub></a><br /><a href="#example-TuanZiGit" title="Examples">💡</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://www.gusui.site/"><img src="https://avatars.githubusercontent.com/u/170245818?v=4?s=100" width="100px;" alt="吕璟辰"/><br /><sub><b>吕璟辰</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=GusuiCommunity" title="Documentation">📖</a> <a href="#promotion-GusuiCommunity" title="Promotion">📣</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/jizilin6732"><img src="https://avatars.githubusercontent.com/u/162853646?v=4?s=100" width="100px;" alt="jizilin6732"/><br /><sub><b>jizilin6732</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=jizilin6732" title="Documentation">📖</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://www.khyan.top/"><img src="https://avatars.githubusercontent.com/u/56215525?v=4?s=100" width="100px;" alt="clover_yan"/><br /><sub><b>clover_yan</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=clover-yan" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/DryIce-cc"><img src="https://avatars.githubusercontent.com/u/165131008?v=4?s=100" width="100px;" alt="DryIce-cc"/><br /><sub><b>DryIce-cc</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=DryIce-cc" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/RoboMico"><img src="https://avatars.githubusercontent.com/u/59791306?v=4?s=100" width="100px;" alt="RoboMico"/><br /><sub><b>RoboMico</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=RoboMico" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="https://github.com/user111192"><img src="https://avatars.githubusercontent.com/u/67212831?v=4?s=100" width="100px;" alt="user111192"/><br /><sub><b>user111192</b></sub></a><br /><a href="#example-user111192" title="Examples">💡</a></td>
    </tr>
    <tr>
      <td align="center" valign="top" width="14.28%"><a href="https://blog.kagureion.top/"><img src="https://avatars.githubusercontent.com/u/141834038?v=4?s=100" width="100px;" alt="一般路过绫音酱_Cecilion"/><br /><sub><b>一般路过绫音酱_Cecilion</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=LyCecilion" title="Documentation">📖</a></td>
      <td align="center" valign="top" width="14.28%"><a href="http://lipoly.ink"><img src="https://avatars.githubusercontent.com/u/110595296?v=4?s=100" width="100px;" alt="LiPolymer"/><br /><sub><b>LiPolymer</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=LiPolymer" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="http://www.mczhiguang.icoc.me/"><img src="https://avatars.githubusercontent.com/u/53519310?v=4?s=100" width="100px;" alt="Amiya"/><br /><sub><b>Amiya</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=mcAmiya" title="Code">💻</a></td>
      <td align="center" valign="top" width="14.28%"><a href="http://xiaowuap.com"><img src="https://avatars.githubusercontent.com/u/44547885?v=4?s=100" width="100px;" alt="Enzo Wu"/><br /><sub><b>Enzo Wu</b></sub></a><br /><a href="https://github.com/ClassIsland/ClassIsland/commits?author=xiaowuap" title="Code">💻</a></td>
    </tr>
  </tbody>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->
<!--markdownlint-disable MD001 MD033 MD041 MD051-->

本项目使用了[这些第三方库和框架](./doc/Dependencies.md)。

感谢 [JetBrains](https://www.jetbrains.com.cn/) 为本项目的开发人员提供的[开源开发许可证](https://www.jetbrains.com.cn/community/opensource/)。

详细的致谢信息请前往[【应用设置】->【关于 ClassIsland】](classisland://app/settings/about)界面中查看。

## 赞助商 / Sponsors

感谢以下赞助商对本项目的支持。

<table>
  <tr>
    <td>
      <img alt="SignPath" src="https://signpath.org/assets/favicon-50x50.png" />
    </td>
    <td>
    Free code signing on Windows provided by <a href="https://signpath.io">SignPath.io</a>, certficate by <a href="https://signpath.org/">SignPath Foundation</a>
    </td>
  </tr>
</table>

## 代码签名策略 / Code signing policy

- Free code signing provided by [SignPath.io](https://about.signpath.io/), certificate by [SignPath Foundation](https://signpath.org/).<br/>
  由 [SignPath.io](https://about.signpath.io/) 提供免费代码签名，由 [SignPath Foundation](https://signpath.org/) 提供证书。
- Committers and reviewers: [Members team](https://github.com/orgs/ClassIsland/teams/members)<br/>
  提交者和审阅者：[团队成员](https://github.com/orgs/ClassIsland/people)
- Approvers: [Owners](https://github.com/orgs/ClassIsland/people?query=role%3Aowner)<br/>
  审批人：[所有者](https://github.com/orgs/ClassIsland/people?query=role%3Aowner)
- [Privacy policy](./doc/Privacy.md)<br/>
 [隐私政策](./doc/Privacy.md)

## 许可证

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FHelloWRC%2FClassIsland.svg?type=shield)](https://app.fossa.com/projects/git%2Bgithub.com%2FHelloWRC%2FClassIsland?ref=badge_shield&style=flat-square)

本项目基于 [MIT License](LICENSE.txt) 获得许可。

[![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FHelloWRC%2FClassIsland.svg?type=large)](https://app.fossa.com/projects/git%2Bgithub.com%2FHelloWRC%2FClassIsland?ref=badge_large)

## Stars 历史

[![Star 历史](https://starchart.cc/ClassIsland/ClassIsland.svg?variant=adaptive)](https://starchart.cc/ClassIsland/ClassIsland)

<div align="center">

如果这个项目对您有帮助，请点亮 Star ⭐

</div>