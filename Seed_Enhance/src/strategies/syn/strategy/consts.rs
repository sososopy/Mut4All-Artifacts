use std::ops::Range;

pub const REMOVE_P: f64 = 0.1;

pub const BLOCK_STMT_R: Range<i64> = 2..10;

pub const MOVE_R: f64 = 0.3;
pub const BREAK_EXPR_B: f64 = 0.1;
pub const RET_EXPR_B: f64 = 0.1;
pub const IF_ELSE_B: f64 = 0.5;
pub const RANGE_LIMITS_B: f64 = 0.5;
pub const RANGE_START_B: f64 = 0.2;
pub const RANGE_END_B: f64 = 0.8;
pub const LIFETIME_B: f64 = 0.1;
pub const LIFETIME_R: Range<usize> = 1..8;
pub const LET_ELSE_B: f64 = 0.1;
pub const FN_CONST_B: f64 = 0.3;
pub const FN_ASYNC_B: f64 = 0.3;
pub const FN_UNSAFE_B: f64 = 0.1;
pub const FN_NAME_R: Range<usize> = 2..8;
pub const PAT_OR_LEAD_B: f64 = 0.05;
pub const PAT_OR_R: Range<usize> = 1..5;
pub const TYPE_PTR_CONST_B: f64 = 0.2;
pub const TYPE_PTR_MUT_B: f64 = 0.2;
pub const VAR_REF_B: f64 = 0.2;
pub const VAR_MUT_B: f64 = 0.2;
pub const FIELDS_R: Range<usize> = 0..3;
pub const ARR_R: Range<usize> = 0..5;