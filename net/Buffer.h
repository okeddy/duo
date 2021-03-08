#ifndef DUO_NET_BUFFER_H
#define DUO_NET_BUFFER_H

#include "../base/copyable.h"

#include <algorithm>
#include <string>
#include <vector>

#include <assert.h>

namespace duo {
    class Buffer : public duo::copyable {
    public:
        static const size_t kCheapPrepend = 8;
        static const size_t kInitialSize = 1024;

        Buffer()
            : buffer_(kCheapPrepend + kInitialSize),
            readerIndex_(kCheapPrepend),
            writerIndex_(kCheapPrepend) {
            assert(readableBytes() == 0);
            assert(writableBytes() == kInitialSize);
            assert(prependableBytes() == kCheapPrepend);
        }

        void swap(Buffer& rhs) {
            buffer_.swap(rhs.buffer_);
            std::swap(readerIndex_, rhs.readerIndex_);
            std::swap(writerIndex_, rhs.writerIndex_);
        }

        size_t readableBytes() const {
            return writerIndex_ - readerIndex_;
        }

        size_t writableBytes() const {
            return buffer_.size() - writerIndex_;
        }

        size_t prependableBytes() const {
            return readerIndex_;
        }

        const char* peek() const {
            return begin() + readerIndex_;
        }

        void retrieve(size_t len) {
            assert(len <= readableBytes());
            readerIndex_ += len;
        }

        void retrieveUntil(const char* end) {
            assert(peek() <= end);
            assert(end <= beginWrite());
            retrieve(end - peek());
        }

        void retrieveAll() {
            readerIndex_ = kCheapPrepend;
            writerIndex_ = kCheapPrepend;
        }

        std::string retrieveAsString() {
            std::string str(peek(), readableBytes());
            retrieveAll();
            return str;
        }

        void append(const std::string& str) {
            append(str.data(), str.length());
        }

        void append(const char* data, size_t len) {
            ensureWriteableBytes(len);
            std::copy(data, data + len, beginWrite());
            hasWritten(len);
        }

        void append(const void* data, size_t len) {
            append(static_cast<const char*>(data), len);
        }

        void ensureWriteableBytes(size_t len) {
            if (writableBytes() < len) {
                makeSpace(len);
            }
            assert(writableBytes() >= len);
        }

        char* beginWrite() {
            return begin() + writerIndex_;
        }

        const char* beginWrite() const {
            return begin() + writerIndex_;
        }

        void hasWritten(size_t len) {
            writerIndex_ += len;
        }

        void prepend(const void* data, size_t len) {
            assert(len <= prependableBytes());
            readerIndex_ -= len;
            const char* d = static_cast<const char*>(data);
            std::copy(d, d + len, begin() + readerIndex_);
        }

        void shrink(size_t reserve) {
            std::vector<char> buf(kCheapPrepend + readableBytes() + reserve);
            std::copy(peek(), peek() + readableBytes(),
                buf.begin() + kCheapPrepend);
            buf.swap(buffer_);
        }

        // Read data directly into buffer.
        // It may implement with readv(2)
        // @return result of read(2), @c errno is saved
        ssize_t readFd(int fd, int* savedErrno);

    private:
        char* begin() {
            return &*buffer_.begin();
        }

        const char* begin() const {
            return &*buffer_.begin();
        }

        void makeSpace(size_t len) {
            if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
                buffer_.resize(len + writerIndex_);
            } else {
                assert(kCheapPrepend < readerIndex_);
                size_t readable = readableBytes();
                std::copy(begin() + readerIndex_, begin() + writerIndex_,
                    begin() + kCheapPrepend);
                readerIndex_ = kCheapPrepend;
                writerIndex_ = readerIndex_ + readable;
                assert(readable == readableBytes());
            }
        }

    private:
        std::vector<char> buffer_;
        size_t readerIndex_;
        size_t writerIndex_;
    };
}

#endif  // DUO_NET_BUFFER_H
