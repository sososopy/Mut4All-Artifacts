namespace std {
    namespace runtime {
        namespace detail {
            struct LogFatal {
                LogFatal(char *, int);
                ~LogFatal() noexcept(0);
            };
        } // namespace detail
        struct Object;
        template <typename> struct ObjectPtr {
            ~ObjectPtr();
        };
        struct ObjectRef {
            ObjectPtr<Object> data_;
        };
    } // namespace runtime
    using runtime::ObjectRef;
    namespace runtime {
        template <typename T> struct Optional {
            T value();
        };
    } // namespace runtime
    using runtime::Optional;
    namespace runtime {
        template <typename> struct Array {};
    } // namespace runtime
    using runtime::Array;
    struct BaseExpr : ObjectRef {};
    struct PrimExpr : BaseExpr {};
    namespace tir {
        struct Trace {};
        struct BlockRV;
        using ExprRV = PrimExpr;
        struct ScheduleNode {
            Optional<Trace> trace();
        };
        struct Schedule {
            ScheduleNode *operator->();
        };
        struct ScheduleRuleNode {
            virtual Array<Schedule> Apply(const Schedule &, const BlockRV &);
        };
        struct ScheduleRule {};
        struct CrossThreadReductionNode : ScheduleRuleNode {
            Array<Schedule> Apply(const Schedule &, const BlockRV &) {
                Schedule tmp_sch;
                GetThreadIdxExtentFromTrace(tmp_sch->trace().value());
            }
            ExprRV GetThreadIdxExtentFromTrace(Trace) {
                ExprRV extent;
                return extent;
                runtime::detail::LogFatal("", 1);
            }
        };
        ScheduleRule ScheduleRuleCrossThreadReduction() { CrossThreadReductionNode(); }
    } // namespace tir
} // namespace std