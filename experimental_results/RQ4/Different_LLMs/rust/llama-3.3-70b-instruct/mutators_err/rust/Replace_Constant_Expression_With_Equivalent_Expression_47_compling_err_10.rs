use rand::Rng;
use syn::{Expr, ExprBinary, BinOp, Lit, ExprLit, token, Stmt};
use proc_macro2::Span;

pub trait Mutator {
    fn mutate(&self, ast: &mut syn::File);
}

struct Replace_Constant_Expression_With_Equivalent_Expression_47;

impl Mutator for Replace_Constant_Expression_With_Equivalent_Expression_47 {
    fn mutate(&self, ast: &mut syn::File) {
        for item in &mut ast.items {
            if let syn::Item::Fn(func) = item {
                for statement in &mut func.block.stmts {
                    if let syn::Stmt::Expr(expr, _) = statement {
                        if let Expr::Lit(lit_expr) = &**expr {
                            if let Lit::Int(int_lit) = &lit_expr.lit {
                                let value = int_lit.base10_parse::<i32>().unwrap();
                                let new_expr = self.generate_equivalent_expression(value);
                                *expr = Box::new(new_expr);
                            }
                        }
                    }
                }
            }
        }
    }
}

impl Replace_Constant_Expression_With_Equivalent_Expression_47 {
    fn generate_equivalent_expression(&self, value: i32) -> Expr {
        let mut rng = rand::thread_rng();
        let op = ["+", "-", "*", "/"][rng.gen_range(0..4)];
        let num1 = rng.gen_range(1..=value);
        let num2 = match op {
            "+" => value - num1,
            "-" => value + num1,
            "*" => value / num1,
            "/" => value * num1,
            _ => unreachable!(),
        };

        let num1_lit = Lit::Int(syn::LitInt::new(num1.to_string(), Span::call_site()));
        let num2_lit = Lit::Int(syn::LitInt::new(num2.to_string(), Span::call_site()));

        let num1_expr = Expr::Lit(ExprLit {
            attrs: vec![],
            lit: num1_lit,
        });

        let num2_expr = Expr::Lit(ExprLit {
            attrs: vec![],
            lit: num2_lit,
        });

        let op_token = match op {
            "+" => BinOp::Add(token::Plus(Span::call_site())),
            "-" => BinOp::Sub(token::Minus(Span::call_site())),
            "*" => BinOp::Mul(token::Star(Span::call_site())),
            "/" => BinOp::Div(token::Slash(Span::call_site())),
            _ => unreachable!(),
        };

        Expr::Binary(ExprBinary {
            attrs: vec![],
            left: Box::new(num1_expr),
            op: op_token,
            right: Box::new(num2_expr),
        })
    }
}