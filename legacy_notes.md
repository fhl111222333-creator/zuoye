# Legacy Notes from Previous Developer

> 以下内容由前同事留下，不保证完全正确。请以实验指导书和测试结果为准。

1. 系统用于处理校园资源预约、报修和维护时间段。
2. 用户类型目前有 student、teacher、admin。
3. 资源类型目前有 room、device，未来可能增加 vehicle。
4. 请求类型目前应该支持 booking 和 repair。
5. student 预约 advanced 资源需要进入 PENDING。
6. teacher 和 admin 可以提交 repair 请求。
7. maintenance.csv 中的时间段内，资源不能被预约。
8. 旧代码能编译，但已经很久没有跑完整测试。

前同事备注：

- `PolicyEngine` 里的规则“差不多能用”，但我没有测边界情况。
- `LegacyBuffer` 是我从以前项目里复制过来的，应该没问题吧。
- CSV 解析器只写了最简单版本，遇到复杂输入可能要改。
