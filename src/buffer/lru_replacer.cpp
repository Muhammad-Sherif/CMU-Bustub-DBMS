// //===----------------------------------------------------------------------===//
// //
// //                         BusTub
// //
// // lru_replacer.cpp
// //
// // Identification: src/buffer/lru_replacer.cpp
// //
// // Copyright (c) 2015-2019, Carnegie Mellon University Database Group
// //
// //===----------------------------------------------------------------------===//

#include "buffer/lru_replacer.h"

namespace bustub {

LRUReplacer::LRUReplacer(size_t num_pages) {
    frame_status_.resize(num_pages , 0);
    frame_iterator_.resize(num_pages);

}

LRUReplacer::~LRUReplacer() = default;

bool LRUReplacer::Victim(frame_id_t *frame_id) { 

    std::scoped_lock<std::mutex> scoped_lock(mutex_);
    if(unpinned_frames_.empty()) {
        frame_id = nullptr;
        return false;
    }
    frame_id_t front_frame = unpinned_frames_.front();
    unpinned_frames_.pop_front();
    *frame_id = front_frame;
    frame_status_[front_frame] = 0;
    return true;

}

void LRUReplacer::Pin(frame_id_t frame_id) {

    std::scoped_lock<std::mutex> scoped_lock(mutex_);

    if (frame_status_[frame_id] == 2 ) return;
    if (frame_status_[frame_id] == 1 ) unpinned_frames_.erase(frame_iterator_[frame_id]);
    frame_status_[frame_id] = 2;

}

void LRUReplacer::Unpin(frame_id_t frame_id) {

    std::scoped_lock<std::mutex> scoped_lock(mutex_);

    if(frame_status_[frame_id] == 0 || frame_status_[frame_id] == 2) { 
      unpinned_frames_.push_back(frame_id);
      frame_iterator_[frame_id] = std::prev(unpinned_frames_.end());
      frame_status_[frame_id] = 1;
    }
    
   
}

size_t LRUReplacer::Size() {
    std::scoped_lock<std::mutex> scoped_lock(mutex_);
    return unpinned_frames_.size(); 
}

}  // namespace bustub
