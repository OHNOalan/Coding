from typing import List


# def lower_bound(nums: List[int], target: int) -> int:
#     left, right = 0, len(nums) - 1  # 闭区间 [left, right]
#     while left <= right:  # 区间不为空
#         # 循环不变量：
#         # nums[left-1] < target
#         # nums[right+1] >= target
#         mid = (left + right) // 2
#         if nums[mid] < target:
#             left = mid + 1  # 范围缩小到 [mid+1, right]
#         else:
#             right = mid - 1  # 范围缩小到 [left, mid-1]
#     return left


# def lower_bound(nums: List[int], target: int) -> int:
#     left = 0
#     right = len(nums)  # 左闭右开区间 [left, right)
#     while left < right:  # 区间不为空
#         # 循环不变量：
#         # nums[left-1] < target
#         # nums[right] >= target
#         mid = (left + right) // 2
#         if nums[mid] < target:
#             left = mid + 1  # 范围缩小到 [mid+1, right)
#         else:
#             right = mid  # 范围缩小到 [left, mid)
#     return left  # 返回 left 还是 right 都行，因为循环结束后 left == right


def lower_bound(nums: List[int], target: int) -> int:
    left, right = -1, len(nums)  # 开区间 (left, right)
    while left + 1 < right:  # 区间不为空
        mid = (left + right) // 2
        # 循环不变量：
        # nums[left] < target
        # nums[right] >= target
        if nums[mid] < target:
            left = mid  # 范围缩小到 (mid, right)
        else:
            right = mid  # 范围缩小到 (left, mid)
    return right
