// CGI API 接口封装
class CGIApi {
    constructor() {
        this.baseURL = '/cgi-bin';
        this.timeout = 5000;
    }

    // 通用请求方法
    async request(endpoint, params = {}) {
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), this.timeout);
        
        try {
            const queryString = new URLSearchParams(params).toString();
            const url = `${this.baseURL}/${endpoint}${queryString ? '?' + queryString : ''}`;
            
            const response = await fetch(url, {
                signal: controller.signal,
                headers: {
                    'Cache-Control': 'no-cache'
                }
            });
            
            clearTimeout(timeoutId);
            
            if (!response.ok) {
                throw new Error(`HTTP ${response.status}`);
            }
            
            const data = await response.json();
            return data;
        } catch (error) {
            clearTimeout(timeoutId);
            
            if (error.name === 'AbortError') {
                throw new Error('请求超时');
            }
            
            console.error('API请求失败:', error);
            return { 
                code: -1, 
                msg: '网络请求失败',
                error: error.message 
            };
        }
    }

    // 设备控制通用方法
    async controlDevice(device, value) {
        return await this.request(device, { value: value });
    }

    // LED控制
    async controlLED(action, brightness = null) {
        const params = { 
            value: action === 'on' ? 1 : 0 
        };
        
        if (brightness !== null) {
            params.brightness = brightness;
        }
        
        return await this.request('led_control', params);
    }

    // 风扇控制
    async controlFan(action, speed = null) {
        const params = { 
            value: action === 'on' ? 1 : 0 
        };
        
        if (speed !== null) {
            params.speed = speed;
        }
        
        return await this.request('fan_control', params);
    }

    // 设置风扇速度
    async setFanSpeed(speed) {
        return await this.request('fan_control', { 
            value: 1, 
            speed: speed 
        });
    }

    // 蜂鸣器控制
    async controlBuzzer(action) {
        return await this.request('buzzer_control', { 
            value: action === 'on' ? 1 : 0 
        });
    }

    // 蜂鸣器鸣叫
    async beepBuzzer() {
        return await this.request('buzzer_control', { 
            value: 1 
        });
    }

    // 获取传感器数据
    async getSensorData() {
        return await this.request('sensor_data');
    }

    // 拍照
    async takeSnapshot() {
        return await this.request('snapshot');
    }

    // 开始录像
    async startRecording() {
        return await this.request('record_control', { 
            action: 'start' 
        });
    }

    // 停止录像
    async stopRecording() {
        return await this.request('record_control', { 
            action: 'stop' 
        });
    }

    // 获取系统状态
    async getSystemStatus() {
        return await this.request('status');
    }

    // 测试连接
    async testConnection() {
        try {
            const startTime = Date.now();
            const result = await this.getSensorData();
            const responseTime = Date.now() - startTime;
            
            return {
                ...result,
                responseTime: responseTime
            };
        } catch (error) {
            return {
                code: -1,
                msg: '连接测试失败',
                error: error.message
            };
        }
    }
}

// 创建全局实例
const cgiApi = new CGIApi();