// 主应用类
class IoTMonitorApp {
    constructor() {
        this.api = cgiApi; // 假设cgiApi已在外部定义
        this.sensorInterval = null;
        this.recordInterval = null;
        this.isConnected = false;
        this.isRecording = false;
        this.recordStartTime = 0;
        this.recordDuration = 0;
        this.retryCount = 0;
        this.maxRetries = 3;
        this.shouldShowRetry = true;
        this.init();
    }

    // 初始化应用
    init() {
        this.bindEvents();
        this.startSensorMonitoring();
        this.updateConnectionStatus();
        console.log('物联网监控系统初始化完成');
    }

    // 绑定事件
    bindEvents() {
        // 设备控制按钮事件
        document.querySelectorAll('.control-btn[data-cmd]').forEach(btn => {
            btn.addEventListener('click', (e) => {
                // 修复：datmd 拼写错误 → dataset.cmd
                const cmd = e.target.dataset.cmd;
                const value = parseInt(e.target.dataset.value);
                this.controlDevice(cmd, value);
            });
        });

        // LED亮度滑块
        const brightnessSlider = document.getElementById('led-brightness');
        const brightnessValue = document.getElementById('brightness-value');
        
        if (brightnessSlider && brightnessValue) {
            brightnessSlider.addEventListener('input', (e) => {
                const value = e.target.value;
                brightnessValue.textContent = value + '%';
            });
            
            brightnessSlider.addEventListener('change', (e) => {
                this.setLEDBrightness(parseInt(e.target.value));
            });
        }

        // 风扇速度滑块
        const fanSpeedSlider = document.getElementById('fan-speed');
        const speedValue = document.getElementById('speed-value');
        
        if(fanSpeedSlider && speedValue) {
            fanSpeedSlider.addEventListener('input', (e) => {
                const value = e.target.value;
                speedValue.textContent = value + '%';
            });
            
            fanSpeedSlider.addEventListener('change', (e) => {
                this.setFanSpeed(parseInt(e.target.value));
            });
        }

        // 蜂鸣器鸣叫按钮
        const buzzerBeepBtn = document.getElementById('buzzer-beep');
        if (buzzerBeepBtn) {
            buzzerBeepBtn.addEventListener('click', () => {
                this.beepBuzzer();
            });
        }

        // 拍照按钮
        const snapshotBtn = document.getElementById('snapshot-btn');
        if (snapshotBtn) {
            snapshotBtn.addEventListener('click', () => {
                this.takeSnapshot();
            });
        }

        // 录像控制
        const recordStartBtn = document.getElementById('record-start-btn');
        const recordStopBtn = document.getElementById('record-stop-btn');
        
        if (recordStartBtn) {
            recordStartBtn.addEventListener('click', () => {
                this.startRecording();
            });
        }
        
        if (recordStopBtn) {
            recordStopBtn.addEventListener('click', () => {
                this.stopRecording();
            });
        }

        // 刷新按钮
        const refreshBtn = document.getElementById('refresh-btn');
        if (refreshBtn) {
            refreshBtn.addEventListener('click', () => {
                this.manualRefresh();
            });
        }

        // 视频流错误处理
        const videoStream = document.getElementById('video-stream');
        if (videoStream) {
            videoStream.addEventListener('error', () => {
                // 修复：缺少分号
                console.error('视频流加载失败', 'error');
            }); // 修复：补全闭合大括号
        }

        // 页面可见性变化事件
        document.addEventListener('visibilitychange', () => {
            if (!document.hidden) {
                this.getSensorData();
            }
        });

        // 网络状态检测
        window.addEventListener('online', () => {
            this.showMessage('网络连接已恢复', 'success');
            this.resetRetryStatus();
            this.getSensorData();
        });

        window.addEventListener('offline', () => {
            this.showMessage('网络连接断开', 'error');
            this.updateConnectionStatus(false);
        });
    }

    // 设备控制
    async controlDevice(device, value) {
        const button = event.target;
        const originalText = button.textContent;
        
        try {
            button.disabled = true;
            button.textContent = '发送中...';
            // 修复：多余空格 + 类名错误 'up ' → 'updating'
            button.classList.add('updating');         
            const result = await this.api.controlDevice(device, value);
            
            if (result.code === 0) {
                this.showMessage('控制命令发送成功', 'success');
                setTimeout(() => this.getSensorData(), 500);
            } else {
                this.showMessage(`控制失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`控制命令发送失败: ${error.message}`, 'error');
        } finally {
            button.disabled = false;
            button.textContent = originalText;
            button.classList.remove('updating');
        }
    }

    // 设置LED亮度
    async setLEDBrightness(brightness) {
        try {
            const result = await this.api.controlLED('on', brightness);
            if (result.code === 0) {
                this.showMessage(`调整为${brightness}%`, 'success');
                const brightnessValue = document.getElementById('brightness-value');
                if (brightnessValue) {
                    brightnessValue.textContent = brightness + '%';
                }
            }
        } catch (error) {
            this.showMessage(`亮度调节失败: ${error.message}`, 'error');
        }
    }

    // 设置风扇速度
    async setFanSpeed(speed) {
        try {
            const result = await this.api.setFanSpeed(speed);
            if (result.code === 0) {
                this.showMessage(`风扇速度调整为${speed}%`, 'success');
                const speedValue = document.getElementById('speed-value');
                if (speedValue) {
                    speedValue.textContent = speed + '%';
                }
            }
        } catch (error) {
            this.showMessage(`风扇速度调节失败: ${error.message}`, 'error');
        }
    }

    // 蜂鸣器鸣叫
    async beepBuzzer() {
        const button = document.getElementById('buzzer-beep');
        if (!button) return;
        
        const originalText = button.textContent;
        
        try {
            button.disabled = true;
            button.textContent = '鸣叫中...';
            button.classList.add('updating');

            const result = await this.api.beepBuzzer();
            if (result.code === 0) {
                this.showMessage('蜂鸣器鸣叫', 'success');
            } else {
                this.showMessage(`蜂鸣器控制失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`蜂鸣器控制失败: ${error.message}`, 'error');
        } finally {
            setTimeout(() => {
                button.disabled = false;
                button.textContent = originalText;
                button.classList.remove('updating');
            }, 1000);
        }
    }

    // 拍照功能
    async takeSnapshot() {
        const button = document.getElementById('snapshot-btn');
        if (!button) return;
        
        const originalText = button.textContent;
        
        try {
            button.disabled = true;
            button.textContent = '拍照中...';
            button.classList.add('updating');

            const result = await this.api.takeSnapshot();
            
            if (result.code === 0) {
                this.showMessage('拍照成功', 'success');
            } else {
                this.showMessage(`拍照失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`拍照错误: ${error.message}`, 'error');
        } finally {
            setTimeout(() => {
                button.disabled = false;
                button.textContent = originalText;
                button.classList.remove('updating');
            }, 2000);
        }
    }

    // 开始录像
    async startRecording() {
        try {
            const result = await this.api.startRecording();
            
            if (result.code === 0) {
                this.isRecording = true;
                this.recordStartTime = Date.now();
                this.startRecordTimer();
                
                const recordStartBtn = document.getElementById('record-start-btn');
                const recordStopBtn = document.getElementById('record-stop-btn');
                const recordingStatus = document.getElementById('recording-status');
                
                if (recordStartBtn) recordStartBtn.disabled = true;
                if (recordStopBtn) recordStopBtn.disabled = false;
                if (recordingStatus) recordingStatus.style.display = 'flex';
                
                this.showMessage('录像已开始', 'success');
            } else {
                this.showMessage(`开始录像失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`开始录像失败: ${error.message}`, 'error');
        }
    }

    // 停止录像
    async stopRecording() {
        try {
            const result = await this.api.stopRecording();
            
            if (result.code === 0) {
                this.isRecording = false;
                this.stopRecordTimer();
                
                const recordStartBtn = document.getElementById('record-start-btn');
                const recordStopBtn = document.getElementById('record-stop-btn');
                const recordingStatus = document.getElementById('recording-status');
                
                if (recordStartBtn) recordStartBtn.disabled = false;
                if (recordStopBtn) recordStopBtn.disabled = true;
                if (recordingStatus) recordingStatus.style.display = 'none';
                
                this.showMessage(`录像已停止，时长: ${this.formatTime(this.recordDuration)}`, 'success');
                this.recordDuration = 0;
            } else {
                this.showMessage(`停止录像失败: ${result.msg}`, 'error');
            }
        } catch (error) {
            this.showMessage(`停止录像失败: ${error.message}`, 'error');
        }
    }

    // 启动录像计时器
    startRecordTimer() {
        this.recordInterval = setInterval(() => {
            this.recordDuration = Date.now() - this.recordStartTime;
            const recordTime = document.getElementById('record-time');
            if (recordTime) {
                recordTime.textContent = this.formatTime(this.recordDuration);
            }
        }, 1000);
    }

    // 停止录像计时器
    stopRecordTimer() {
        if (this.recordInterval) {
            clearInterval(this.recordInterval);
            this.recordInterval = null;
        }
    }

    // 格式化时间
    formatTime(milliseconds) {
        const totalSeconds = Math.floor(milliseconds / 1000);
        const minutes = Math.floor(totalSeconds / 60);
        const seconds = totalSeconds % 60;
        return `${minutes.toString().padStart(2, '0')}:${seconds.toString().padStart(2, '0')}`;
    }

    // 手动刷新
    manualRefresh() {
        const button = document.getElementById('refresh-btn');
        if (!button) return;
        
        const originalText = button.textContent;
        
        button.disabled = true;
        button.textContent = '刷新中...';
        button.classList.add('updating');

        this.getSensorData().finally(() => {
            setTimeout(() => {
                button.disabled = false;
                button.textContent = originalText;
                button.classList.remove('updating');
            }, 1000);
        });
    }

    // 获取传感器数据
    async getSensorData() {
        try {
            this.setLoadingState(true);
            
            const data = await this.api.getSensorData();
            
            if (data.code === 0) {
                this.updateSensorDisplay(data);
                this.updateConnectionStatus(true);
                this.isConnected = true;
                this.resetRetryStatus();
                this.retryCount = 0;
            } else {
                this.updateConnectionStatus(false);
                this.isConnected = false;
                this.handleConnectionError(data.msg);
            }
        } catch (error) {
            console.error('获取传感器数据失败:', error);
            this.updateConnectionStatus(false);
            this.isConnected = false;
            this.handleConnectionError(error.message);
        } finally {
            this.setLoadingState(false);
        }
    }

    // 设置加载状态
    setLoadingState(loading) {
        const statusArea = document.querySelector('.status-area');
        if (statusArea) {
            if (loading) {
                statusArea.classList.add('updating');
            } else {
                statusArea.classList.remove('updating');
            }
        }
    }

    // 处理连接错误
    handleConnectionError(errorMsg) {
        this.retryCount++;
        
        if (this.retryCount <= this.maxRetries) {
            if (this.shouldShowRetry) {
                // 修复：this.Count 拼写错误 → this.retryCount
                this.showMessage(`连接异常，正在重试... (${this.retryCount}/${this.maxRetries})`, 'warning');
            }
            
            const delay = Math.min(1000 * Math.pow(2, this.retryCount - 1), 10000);
            setTimeout(() => {
                if (this.retryCount <= this.maxRetries) {
                    this.getSensorData();
                }
            }, delay);
        } else {
            this.shouldShowRetry = false;
            this.showMessage('设备连接失败，请检查硬件连接', 'error');
            this.updateConnectionStatus(false);
        }
    }

    // 重置重试状态
    resetRetryStatus() {
        this.retryCount = 0;
        this.shouldShowRetry = true;
    }

    // 更新传感器显示
    updateSensorDisplay(data) {
        this.updateValue('temp', data.temp, '°C');
        this.updateValue('humi', data.humi, '%');
        this.updateValue('light', data.light, 'lux');
        
        this.updateDeviceStatus('led-status', data.led_status, ['关闭', '开启']);
        this.updateDeviceStatus('buzzer-status', data.buzzer_status, ['关闭', '开启']);
        
        this.updateFanStatus(data.fan_status, data.fan_speed);
        
        this.updateTimestamp(data.timestamp);
        this.updateStatusIndicators();
    }

    // 更新数值显示
    updateValue(elementId, value, unit) {
        const element = document.getElementById(elementId);
        if (element && value !== undefined && value !== null) {
            if (typeof value === 'number') {
                element.textContent = value.toFixed(1);
            } else {
                element.textContent = value;
            }
            
            this.applyValueStyling(elementId, value);
        }
    }

    // 根据数值范围应用样式
    applyValueStyling(elementId, value) {
        const element = document.getElementById(elementId);
        if (!element) return;
        
        const statusItem = element.closest('.status-item');
        if (!statusItem) return;

        statusItem.classList.remove('status-normal', 'status-warning', 'status-critical');

        if (elementId === 'temp') {
            if (value > 35 || value < 10) {
                statusItem.classList.add('status-critical');
            } else if (value > 30 || value < 15) {
                statusItem.classList.add('status-warning');
            } else {
                statusItem.classList.add('status-normal');
            }
        } else if (elementId === 'humi') {
            if (value > 80 || value < 30) {
                statusItem.classList.add('status-critical');
            } else if (value > 70 || value < 40) {
                statusItem.classList.add('status-warning');
            } else {
                statusItem.classList.add('status-normal');
            }
        } else if (elementId === 'light') {
            if (value < 100) {
                statusItem.classList.add('status-warning');
            } else {
                statusItem.classList.add('status-normal');
            }
        }
    }

    // 更新设备状态
    updateDeviceStatus(elementId, status, statusTexts) {
        const element = document.getElementById(elementId);
        if (element && status !== undefined) {
            element.textContent = statusTexts[status] || '未知';
            element.style.color = status ? '#4caf50' : '#f44336';
        }
    }

    // 更新风扇状态
    updateFanStatus(status, speed) {
        const statusElement = document.getElementById('fan-status');
        const speedElement = document.getElementById('fan-speed-display');
        
        if (statusElement) {
            statusElement.textContent = status ? '开启' : '关闭';
            statusElement.style.color = status ? '#4caf50' : '#f44336';
            
            if (speedElement) {
                // 修复：未定义变量speedContent → 直接赋值给speedElement.textContent
                if (status && speed > 0) {
                    speedElement.textContent = `(${speed}%)`;
                } else {
                    speedElement.textContent = '';
                }
            }
        }
    }

    // 更新时间戳
    updateTimestamp(timestamp) {
        const element = document.getElementById('last-update');
        if (element) {
            const date = timestamp ? new Date(timestamp * 1000) : new Date();
            element.textContent = `最后更新: ${date.toLocaleTimeString()}`;
        }
    }

    // 更新状态指示器
    updateStatusIndicators() {
        const sysStatusElement = document.getElementById('sys-status');
        if (!sysStatusElement) return;
        
        let indicator = sysStatusElement.querySelector('.status-indicator');
        
        if (!indicator) {
            indicator = document.createElement('span');
            indicator.className = 'status-indicator';
            sysStatusElement.prepend(indicator);
        }

        // 修复：类名拼接错误 + 空格问题
        indicator.className = 'indicator ' + (this.isConnected ? 'status-online' : 'status-offline');
        
        // 修复：textContent会覆盖prepend的indicator → 先保存indicator，再设置文本，最后重新添加
        const text = this.isConnected ? '正常运行' : '连接异常';
        sysStatusElement.textContent = text;
        sysStatusElement.style.color = this.isConnected ? '#4caf50' : '#f44336';
        sysStatusElement.prepend(indicator); // 重新添加指示器
    }

    // 更新连接状态
    updateConnectionStatus(connected = null) {
        if (connected !== null) {
            this.isConnected = connected;
        }
        
        const sysStatusElement = document.getElementById('sys-status');
        if (sysStatusElement) {
            if (this.isConnected) {
                sysStatusElement.textContent = '正常运行';
                sysStatusElement.style.color = '#4caf50';
            } else {
                sysStatusElement.textContent = '连接异常';
                sysStatusElement.style.color = '#f44336';
            } // 修复：补全闭合大括号
        }
    }

    // 显示消息
    showMessage(message, type = 'info') { // 修复：方法定义格式错误
        this.removeExistingMessages();
        
        const messageDiv = document.createElement('div');
        messageDiv.className = `message message-${type}`;
        messageDiv.innerHTML = `
            <span class="message-icon">${this.getMessageIcon(type)}</span>
            <span class="message-text">${message}</span>
            <button class="message-close">&times;</button>
        `;
        
        const container = document.querySelector('.container');
        if (container) {
            container.insertBefore(messageDiv, container.firstChild);
            
            messageDiv.querySelector('.message-close').addEventListener('click', () => {
                messageDiv.remove();
            });
            
            if (type === 'info') {
                setTimeout(() => {
                    if (messageDiv.parentNode) {
                        messageDiv.remove();
                    }
                }, 3000);
            }
        }
    }

    // 移除现有消息
    removeExistingMessages() { // 修复：注释和方法定义格式错误
        document.querySelectorAll('.message').forEach(msg => msg.remove());
    }

    // 获取消息图标
    getMessageIcon(type) {
        const icons = {
            'success': '✅',
            'error': '❌',
            'warning': '⚠️',
            'info': 'ℹ️'
        };
        return icons[type] || '💬';
    }

    // 启动传感器监控
    startSensorMonitoring() {
        this.getSensorData();
        
        this.sensorInterval = setInterval(() => {
            if (!document.hidden) {
                this.getSensorData();
            }
        }, 3000);
    }

    // 停止监控
    stopMonitoring() {
        if (this.sensorInterval) {
            clearInterval(this.sensorInterval);
            this.sensorInterval = null;
        } // 修复：缩进和大括号匹配错误
        if (this.recordInterval) {
            clearInterval(this.recordInterval);
            this.recordInterval = null;
        }
    }

    // 销毁实例
    destroy() {
        this.stopMonitoring(); // 停止监控传感器数据
        this.removeExistingMessages(); // 移除所有消息
    }
}

// 页面加载完成后初始化应用
document.addEventListener('DOMContentLoaded', () => {
    window.iotApp = new IoTMonitorApp();
});

// 页面卸载时清理资源
window.addEventListener('beforeunload', () => {
  if (window.iotApp) {
    window.iotApp.destroy();
  }
}); // 修复：全角分号；→ 半角分号;