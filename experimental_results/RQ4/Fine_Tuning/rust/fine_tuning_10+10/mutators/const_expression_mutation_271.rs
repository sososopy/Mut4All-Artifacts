use crate::mutator::Mutator;
use syn::spanned::Spanned;
use syn::{visit_mut::VisitMut, Expr, BinOp, Item, GenericParam, Token};

pub struct Const_Expression_Mutation_271;

impl Mutator for Const_Expression_Mutation_271 {
    fn name(&self) -> &str {
        "Const_Expression_Mutation_271"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ConstExprMutator;

        impl VisitMut for ConstExprMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Binary(expr_binary) = expr {
                    if let Expr::Path(expr_path) = &*expr_binary.left {
                        if expr_path.path.segments.len() == 1 {
                            let segment = &expr_path.path.segments[0];
                            if segment.ident.to_string().chars().all(|c| c.is_uppercase()) {
                                let new_op = match expr_binary.op {
                                    BinOp::Add(_) => BinOp::Sub(Token![-](expr_binary.op.span())),
                                    BinOp::Sub(_) => BinOp::Add(Token![+](expr_binary.op.span())),
                                    BinOp::Mul(_) => BinOp::Div(Token![/](expr_binary.op.span())),
                                    BinOp::Div(_) => BinOp::Mul(Token![*](expr_binary.op.span())),
                                    _ => return,
                                };
                                expr_binary.op = new_op;
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(_) = param {
                        ConstExprMutator.visit_block_mut(&mut item_fn.block);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions with const generic parameters, altering arithmetic operations involving these parameters. This is achieved by changing operators (e.g., addition to subtraction) within const expressions, ensuring the robustness of const generic handling."
    }
}