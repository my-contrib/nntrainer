// SPDX-License-Identifier: Apache-2.0
/**
 * Copyright (C) 2020 Parichay Kapoor <pk.kapoor@samsung.com>
 *
 * @file   manager.h
 * @date   30 Nov 2020
 * @brief  This is NNtrainer manager for all weights, i/o and intermediate
 * tensors
 * @see    https://github.com/nnstreamer/nntrainer
 * @author Parichay Kapoor <pk.kapoor@samsung.com>
 * @author Jihoon Lee <jhoon.it.lee@samsung.com>
 * @bug	   No known bugs except for NYI items
 *
 */

#ifndef __MANAGER_H__
#define __MANAGER_H__
#ifdef __cplusplus

#include <functional>
#include <memory>
#include <vector>

#include <weight.h>

namespace nntrainer {

/**
 * @class MMappedMemory
 * @brief Memory Handler, that has mmaped memory with a file descriptor
 */
class MMapedMemory {
public:
  /**
   * @brief Construct a new MMapedMemory object
   *
   * @param size bytesize of the memory chunk
   * @param allocate_fd map a shared memory object to a file
   */
  MMapedMemory(size_t size, bool allocate_fd = false);

  ~MMapedMemory() noexcept;

  MMapedMemory(const MMapedMemory &) = delete;

  MMapedMemory &operator=(const MMapedMemory &) = delete;

  /**
   * @brief Get the File descriptor.
   * Will return -1 except for android
   * @todo make this available for other platforms
   *
   * @return -1 if fd is not allocated (or unabled to allocate)
   */
  int getFd() noexcept { return fd; }

  size_t size() noexcept { return buf_size; }

  /**
   * @brief get Typed buffer from the memory
   *
   * @tparam T Type to specify the buffer. return is reinterpreted to T*
   * @return T* Typed buffer, return nullptr if empty
   */
  template <typename T> T *typedBuffer() noexcept {
    return reinterpret_cast<T *>(buf);
  }

  void *data() noexcept { return typedBuffer<void>(); }

private:
  bool allocate_fd; /**< option to choose to allocate an fd */
  int fd;           /**< fd to access the shared_memory  */
  void *buf;        /**< buffer object when use_shared_memory */
  size_t buf_size;  /**< buffer size */
};

/**
 * @class   Manager
 * @brief   manager of nntrainer
 */
class Manager {

public:
  /**
   * @brief     Constructor of Manager
   */
  Manager(bool enable_gradient_memory_opt_ = true,
          bool use_shared_memory_ = true);

  Manager(const Manager &) = delete;

  Manager &operator=(const Manager &) = delete;

  Manager(Manager &&) noexcept = default;

  Manager &operator=(Manager &&) noexcept = default;

  /**
   * @brief     Destructor of Manager
   */
  ~Manager();

  /**
   * @brief     Add weight to be tracked and updated with nntrainer
   *
   * @param w   Weight to be tracked
   */
  void trackWeight(std::reference_wrapper<Weight> w);

  /**
   * @brief     Add weights to be tracked and updated with nntrainer
   *
   * @param ws  Weights to be tracked
   */
  void trackWeights(std::vector<Weight> &ws);

  /**
   * @brief     Get weights tracked with nntrainer
   *
   * @retval    list of weight references
   */
  std::vector<std::vector<std::reference_wrapper<Weight>>> getWeightRefs() {
    return weights;
  }

  /**
   * @brief Enable gradient memory sharing based optimization
   * @param opt True to enable, else false
   */
  void setGradientMemoryOptimization(bool opt) {
    enable_gradient_memory_opt = opt;
  }

  /**
   * @brief Allocate and initialize the weight variable
   */
  void initialize();

  /**
   * @brief Reset the manager state
   */
  void reset() {
    weights.clear();
    max_grad_size = 0;
    total_weight_size = 0;
    total_grad_size = 0;
    weight_mmaped_memory.reset();
    grad_mmaped_memory.reset();
  }

private:
  // TODO: ensure that names of these weights are unique
  /**< Weights all the layer in the model to be managed */
  std::vector<std::vector<std::reference_wrapper<Weight>>> weights;

  size_t total_weight_size; /**< total weight size */
  size_t total_grad_size;   /**< total weight size */
  size_t max_grad_size;     /**< max trainable weight required by a layer */

  bool enable_gradient_memory_opt; /**< share memory among all the gradients */

  /**< shared memory related */
  bool use_shared_memory; /**< uses shared memory object which is owned by
                             manager */
  std::unique_ptr<MMapedMemory> weight_mmaped_memory;
  std::unique_ptr<MMapedMemory> grad_mmaped_memory;
};

} // namespace nntrainer

#endif /* __cplusplus */
#endif /* __MANAGER_H__ */
