#pragma once
#include <string>

// 定义消息类型
enum class MessageType {
    LOGIN_REQUEST,
    LOGIN_RESPONSE,
    GET_PRODUCTS,
    PRODUCTS_RESPONSE,
    SEARCH_PRODUCTS,
    SEARCH_RESPONSE,
    ADD_TO_CART,
    GET_CART,
	Get_CART_RESPONSE,
    REMOVE_FROM_CART,
    CART_RESPONSE,
    CREATE_ORDER,
    ORDER_RESPONSE,
    PAY_ORDER,
    PAYMENT_RESPONSE,
	Error,
};

// 定义消息结构
struct Message {
    MessageType type;
    std::string data;  // JSON格式的数据
};

// 定义错误码
enum class ErrorCode {
    SUCCESS = 0,
    LOGIN_FAILED,
    PRODUCT_NOT_FOUND,
    INSUFFICIENT_STOCK,
    PAYMENT_FAILED,
    NETWORK_ERROR
};