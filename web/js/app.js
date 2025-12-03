// 主应用类
class IoTMonitorApp {
    constructor() {
        this.api = cgiApi;
        this.sensorInterval = null;
        this.init();
    }

    // 初始化应用
    init() {
        this.bindEvents();
        this.startSensorMonitoring();
    }

    // 绑定事件
    bindEvents() {
        // 设备控制按钮事件
        document.querySelectorAll('.control-btn[data-cmd]').forEach(btn => {
            btn.addEventListener('click', (e) => {
                const cmd = e.target.dataset.cmd;
                const value = e.target.dataset.value;
                this.controlDevice(cmd, value);
            });
        });

        // 拍照按钮事件
        document.getElementById('snapshot-btn').addEventListener('click', () => {
            this.takeSnapshot();
        });

        // 视频流错误处理
        document.getElementById('video-stream').addEventListener('error', (e) => {
            console.error('视频流加载失败');
            e.target.alt = '视频流加载失败，请检查摄像头连接';
        });
    }

    // 设备控制
    async controlDevice(cmd, value) {
        try {
            const result = await this.api.controlDevice(cmd, value);
            if (result.code === 0) {
                this.showMessage(`${cmd} 操作成功`, 'success');
            } else {
                this.showMessage(`操作失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`通信错误: ${error.message}`, 'error');
        }
    }

    // 拍照功能
    async takeSnapshot() {
        try {
            const result = await this.api.snapshot();
            if (result.code === 0) {
                this.showMessage(`拍照成功，保存路径: ${result.path}`, 'success');
            } else {
                this.showMessage(`拍照失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`拍照错误: ${error.message}`, 'error');
        }
    }

    // 获取传感器数据
    async getSensorData() {
        try {
            const data = await this.api.getSensorData();
            if (data.type === 'sensor') {
                this.updateSensorDisplay(data);
                this.updateSystemStatus('正常运行');
            }
        } catch (error) {
            this.updateSystemStatus('连接异常');
            console.error('获取传感器数据失败:', error);
        }
    }

    // 更新传感器显示
    updateSensorDisplay(data) {
        document.getElementById('temp').textContent = data.temp || '-';
        document.getElementById('humi').textContent = data.humi || '-';
        
        const smokeElement = document.getElementById('smoke');
        const smokeAlert = document.getElementById('smoke-alert');
        
        if (data.smoke) {
            smokeElement.textContent = '检测到';
            smokeAlert.textContent = '⚠️ 警告：检测到烟雾';
        } else {
            smokeElement.textContent = '正常';
            smokeAlert.textContent = '';
        }
    }

    // 更新系统状态
    updateSystemStatus(status) {
        document.getElementById('sys-status').textContent = status;
    }

    // 显示消息
    showMessage(message, type = 'info') {
        // 可以在这里实现更复杂的消息提示
        alert(`${type.toUpperCase()}: ${message}`);
    }

    // 启动传感器监控
    startSensorMonitoring() {
        // 立即执行一次
        this.getSensorData();
        // 每3秒更新一次
        this.sensorInterval = setInterval(() => {
            this.getSensorData();
        }, 3000);
    }

    // 停止监控
    stopMonitoring() {
        if (this.sensorInterval) {
            clearInterval(this.sensorInterval);
        }
    }
}

// 页面加载完成后初始化应用
document.addEventListener('DOMContentLoaded', () => {
    window.iotApp = new IoTMonitorApp();
});