# UltraSonicHMI_simplified

基于 Qt 5 Widgets 的超声采集上位机。它通过 UDP 接收采集板卡数据，可显示 12 路波形，切换到 FFT 频谱和滤波视图，也支持串口连接、保存和回放历史波形，以及 SEG-2、SEG-Y 导出。简化版构建仅包含上位机与波形处理功能。

## 环境要求

| 组件 | 版本与来源 |
| --- | --- |
| 操作系统 | 64 位 Windows |
| 环境管理 | Miniconda |
| FFTW | conda-forge 的 3.3.10 非 MPI 版本 |
| Qt SDK | Qt 5.15.2，使用 Qt 下载站的 64 位 MinGW 套件，包含 Qt Serial Port |
| 工具链 | Qt 下载站的 MinGW-w64 8.1.0，包含 qmake 构建所需的 GCC 和 `mingw32-make` |

Miniconda 管理 FFTW、Python 和 aqtinstall。aqtinstall 将 Qt 和 MinGW 安装到同一 Conda 环境下的 `qt-sdk` 目录。Python 用于安装 SDK，上位机本身是 C++ 程序。首次安装需要联网和数 GB 可用空间。Qt 5 的 Windows 构建工具在含中文的路径中可能找不到文件，请把项目和 Conda 环境放在 ASCII 路径下，例如 D:\projects 和 D:\conda-envs。

## 创建环境

在项目根目录打开已初始化 Conda 的 PowerShell。也可以在 Miniconda Prompt 激活环境后输入 `powershell`，让 PowerShell 继承当前环境。

```powershell
conda env create -f environment.yml
conda activate ultrasonic-hmi

$qtRoot = Join-Path $env:CONDA_PREFIX 'qt-sdk'
python -m aqt install-qt windows desktop 5.15.2 win64_mingw81 --outputdir $qtRoot --archives qtbase qtserialport
python -m aqt install-tool windows desktop tools_mingw81 qt.tools.win64_mingw810 --outputdir $qtRoot
```

若希望把环境放到自选目录，在创建环境一行加上 `--prefix <绝对路径>`，随后用 `conda activate <绝对路径>` 激活。已有环境可用 `conda env update -f environment.yml` 更新。

## 构建与运行

在已激活的 PowerShell 中执行下面的命令。脚本在独立构建目录生成 qmake 产物，并将 Qt、MinGW 和 FFTW 运行文件复制到可执行文件同目录。

```powershell
.\build.ps1
.\build.ps1 -Run
```

默认生成 `build/release/UltraSonicHMI.exe`。可用 `.\build.ps1 -Configuration Debug` 生成调试版，或加上 `-Jobs 2` 减少并发编译。已有相同版本的 Qt 和 MinGW 时，可通过 `-QtRoot` 指定同时包含 `5.15.2` 和 `Tools` 的 SDK 目录。构建完成后可以直接双击 exe。移动程序时请保留相邻的 DLL、Qt 插件目录和 `qt.conf`。

使用 Qt Creator 时，可从已激活的 Conda 环境打开 `UltraSonicHMI/UltraSonicHMI.pro`，为其选择 Qt 5.15.2 / MinGW 8.1 x64 Kit 并使用独立构建目录。若 Creator 独立启动，可在 qmake 参数中通过 `FFTW_PREFIX` 指定 Conda 环境目录，同时在运行环境的 `PATH` 中加入该目录下的 `Library/bin`。

## 离线波形演示

构建完成后，双击 UltraSonicHMI.exe，在「波形显示」页点击「加载」，选择与 exe 同目录的 ultrasonic-demo.hex。项目的 examples 目录中也有原样本。回放可显示 12 路带模拟回波的时域波形，随后点击「FFT关」可切换至频谱。

样本由 examples/generate_demo.py 生成，包含 12 路、每路 4096 点，大小为 98,584 字节。文件是程序可读取的二进制 HEX 帧，振幅和回波均为合成数据。横轴沿用上位机当前配置的采样间隔。

## 基本使用

- 主窗口可以在未连接采集板卡时打开。接入板卡前，在界面确认目标地址、目标端口和本机监听端口，使其匹配设备配置。
- 串口功能需要选择实际设备的端口号和波特率。可用系统设备管理器核对 COM 端口。
- 回放时选择由本程序保存的十二通道 `.hex` 文件。文件保存和首次生成的 `UltraSonicHMI.ini` 位于可执行文件所在目录。
- 若 qmake 找不到 FFTW，先确认当前 Shell 已激活环境。若报告 Qt 或 MinGW 缺失，检查 SDK 安装命令的目标目录是否与脚本使用的目录一致。

项目源码位于 `UltraSonicHMI/`。内置 QCustomPlot 2.1.0 的版权与 GPL-3.0-or-later 声明保留在上游源码中，许可正文见 [QCustomPlot GPL-3.0](LICENSES/QCustomPlot-GPL-3.0.txt)。
