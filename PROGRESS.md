# 项目进度

## 2026-11-26 · Windows 环境与首次交付

- 已将 Windows x64 的构建依赖写入 environment.yml。Miniconda 管理 FFTW 3.3.10 和 aqtinstall，Qt 5.15.2、Qt Serial Port 与 MinGW-w64 8.1.0 安装在环境的 qt-sdk 子目录。
- 已更新 qmake 项目和 build.ps1，支持独立的 Release、Debug 构建目录。界面头文件随构建重新生成，FFTW 从 Conda 环境加载。
- 已补齐 README 的安装、运行和使用步骤，保留 QCustomPlot 的上游许可声明与许可正文。原始文件包里的构建产物、第三方工具、参考附件和未参与简化版构建的 MATLAB 组件保留在本地。
- 已用 environment.yml 重建环境并通过 pip check。原 Qt 下载镜像传输停滞后，从可用镜像取得 Qt 和 MinGW 归档，并与 Qt 下载站提供的 SHA-256 核对。
- Windows Release、Debug 构建通过。从仅含待发布文件的独立目录重新构建 Release 通过，验证路径含空格。三个构建产物均通过离屏启动和本机 UDP 回环检查。使用 4096 点已知信号独立验证了 Conda FFTW 的频谱峰值。
- 实际采集板卡、串口设备、波形回放和文件导出待联调。旧版 Qt/MinGW 对包含中文的构建路径存在兼容问题，README 记录了可用的路径设置。原有源码仍有编译警告。
