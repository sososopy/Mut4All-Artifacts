use std::error::Error;
use syn::{BinOp, Expr, ExprBinary};

use crate::{
    do_gen_name,
    fuzz::rand_choose::WeightedRand,
    register_strategy,
    strategies::syn::strategy::{info::env::ASTEnv, FuzzerStrategyImpl},
    use_nodetype,
};

use_nodetype!(syn, Expr);

pub struct ExprBinaryStrategy {
    rnd: WeightedRand<usize, f64>,
}
impl Default for ExprBinaryStrategy {
    fn default() -> Self {
        let all_op = vec![
            1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, 26, 27, 28,
        ];
        let all: Vec<_> = all_op.into_iter().map(|x| (x, 1.)).collect();
        Self {
            rnd: WeightedRand::new(Some(all), None),
        }
    }
}
impl FuzzerStrategyImpl<Expr> for ExprBinaryStrategy {
    // fn do_fuzz(&self, v: &mut dyn Fold, _node: Expr) -> Result<Expr, Box<dyn Error>> {
    //     let mut node = if let Expr::Binary(node) = _node {
    //         node
    //     } else {
    //         return Err("Not a binary expression".into());
    //     };
    //     if let BinOp::Add(_) = node.op {
    //         node.op = BinOp::Mul(Default::default());
    //     }
    //     let res = fold_expr_binary(v, node);
    //     Ok(Expr::Binary(res))
    // }
    fn do_gen(&mut self) -> Result<Expr, Box<dyn Error>> {
        let le = do_gen_name!(Expr)?.do_gen()?;
        let re = do_gen_name!(Expr)?.do_gen()?;
        let op_rnd = self.rnd.rand()?.0;
        let op = match op_rnd {
            1 => BinOp::Add(Default::default()),
            2 => BinOp::Sub(Default::default()),
            3 => BinOp::Mul(Default::default()),
            4 => BinOp::Div(Default::default()),
            5 => BinOp::Rem(Default::default()),
            6 => BinOp::And(Default::default()),
            7 => BinOp::Or(Default::default()),
            8 => BinOp::BitXor(Default::default()),
            9 => BinOp::BitAnd(Default::default()),
            10 => BinOp::BitOr(Default::default()),
            11 => BinOp::Shl(Default::default()),
            12 => BinOp::Shr(Default::default()),
            13 => BinOp::Eq(Default::default()),
            14 => BinOp::Lt(Default::default()),
            15 => BinOp::Le(Default::default()),
            16 => BinOp::Ne(Default::default()),
            17 => BinOp::Ge(Default::default()),
            18 => BinOp::Gt(Default::default()),
            19 => BinOp::AddAssign(Default::default()),
            20 => BinOp::SubAssign(Default::default()),
            21 => BinOp::MulAssign(Default::default()),
            22 => BinOp::DivAssign(Default::default()),
            23 => BinOp::RemAssign(Default::default()),
            24 => BinOp::BitXorAssign(Default::default()),
            25 => BinOp::BitAndAssign(Default::default()),
            26 => BinOp::BitOrAssign(Default::default()),
            27 => BinOp::ShlAssign(Default::default()),
            28 => BinOp::ShrAssign(Default::default()),
            _ => BinOp::Add(Default::default()),
        };
        let new_bin = ExprBinary {
            attrs: Vec::new(),
            left: le.into(),
            op,
            right: re.into(),
        };
        Ok(Expr::Binary(new_bin))
    }
    fn gen_weight(&self) -> fn(&ASTEnv) -> f64 {
        |e| 1. / (5 * e.expr_nest) as f64
    }
}

register_strategy!(ExprBinaryStrategy, Expr, Expr::Binary(_));
