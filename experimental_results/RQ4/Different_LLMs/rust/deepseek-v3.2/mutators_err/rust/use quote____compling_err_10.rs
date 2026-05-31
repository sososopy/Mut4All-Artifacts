use proc_macro2::{Span,TokenStream};
use syn::{visit_mut::VisitMut, *};
use quote::{quote, ToTokens};

struct Mutator;

impl Mutator {
    fn mutate(&self, ast: &mut File) {
        let mut visitor = MyVisitor;
        visitor.visit_file_mut(ast);
    }
}

struct MyVisitor;

impl VisitMut for MyVisitor {
    fn visit_expr_mut(&mut self, node: &mut Expr) {
        if let Expr::Binary(bin_expr) = node {
            if let BinOp::Add(_) = bin_expr.op {
                bin_expr.op = BinOp::Sub(Token![-](Span::call_site()));
            }
        }
        syn::visit_mut::visit_expr_mut(self, node);
    }
}