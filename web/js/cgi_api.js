// CGI API 接口封装
class CGIApi {
    constructor() {
        this.baseURL = '/cgi-bin';
    }

    // 通用请求方法
    async request(endpoint, params = {}) {
        try {
            const queryString = new URLSearchParams(params).toString();
            const url = `${this.baseURL}/${endpoint}.cgi${queryString ? '?' + queryString : ''}`;// 示例参数，根据实际API调整
            
            const response = await fetch(url);
            return await response.json();
        } catch (error) {
            console.error('API请求失败:', error);
            return { code: -1, msg: '网络请求失败' };
        }
    }

    // 设备控制
    async controlDevice(cmd, value) {
        return await this.request(cmd, { value });// 示例参数，根据实际API调整
    }

    // 拍照
    async snapshot() {
        return await this.request('snapshot');// 示例参数，根据实际API调整
    }

    // 获取传感器数据
    async getSensorData() {
        return await this.request('get_sensor');// 示例参数，根据实际API调整
    }

    // 获取系统状态
    async getSystemStatus() {
        return await this.request('get_status');// 示例参数，根据实际API调整
    }
}

// 创建全局实例
const cgiApi = new CGIApi();