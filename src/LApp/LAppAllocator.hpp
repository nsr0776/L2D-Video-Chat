/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#ifndef LAPP_ALLOCATOR_HPP_
#define LAPP_ALLOCATOR_HPP_

#include <CubismFramework.hpp>
#include <ICubismAllocator.hpp>

/**
* @brief Memory allocator that implements the ICubismAllocator interface
*
* Memory allocation & deallocation.
* Required by the Cubism Framework.
*
*/
class LAppAllocator : public Csm::ICubismAllocator
{
    /**
    * @brief  Allocate a memory buffer of designated size
    *
    * @param[in]   size    Size to allocate.
    * @return  Allocated buffer pointer.
    */
	void* Allocate(const Csm::csmSizeType size) override;

    /**
    * @brief   Deallocate memory
    *
    * @param[in]   memory    The memory to deallocate
    */
	void Deallocate(void* memory) override;

    /**
    * @brief   Allocate a memory buffer of designated size & alignment
    *
    * @param[in]   size         Size to set.
    * @param[in]   alignment    Alignment to set.
    * @return  Allocated buffer pointer
    */
	void* AllocateAligned(const Csm::csmSizeType size, const Csm::csmUint32 alignment) override;

    /**
    * @brief    Deallocate aligned memory
    *
    * @param[in]   alignedMemory    The aligned memory to deallocate
    */
	void DeallocateAligned(void* alignedMemory) override;
};

#endif