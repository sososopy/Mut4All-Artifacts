use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Array_Const_Expr_In_Impl_368;

impl Mutator for Replace_Array_Const_Expr_In_Impl_368 {
    fn name(&self) -> &str {
        "Replace_Array_Const_Expr_In_Impl_368"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = ArrayConstExprVisitor {
            rng: &mut rng,
            changed: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array type definitions within impl blocks that involve generic constant expressions, specifically those using const generics in array sizes. It applies when an array type in the return position or within a trait bound uses a constant expression involving multiplication, addition, subtraction, or division of generic const parameters. The operator modifies the constant expression by altering the arithmetic operation or one operand while ensuring the expression remains syntactically valid and uses only existing generic const parameters from the context. This aims to trigger bugs related to const expression evaluation, type checking, or trait resolution mismatches in the compiler, especially under the F-generic_const_exprs feature."
    }
}

struct ArrayConstExprVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    changed: bool,
}

impl<'a> VisitMut for ArrayConstExprVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(type_array) = ty {
            if let Expr::Binary(expr_bin) = &mut type_array.len {
                self.mutate_binary_expr(expr_bin);
            } else if let Expr::Path(expr_path) = &mut type_array.len {
                self.mutate_single_operand(expr_path);
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }

    fn visit_item_impl_mut(&mut self, i: &mut ItemImpl) {
        let const_params: HashSet<Ident> = i
            .generics
            .params
            .iter()
            .filter_map(|p| match p {
                GenericParam::Const(c) => Some(c.ident.clone()),
                _ => None,
            })
            .collect();

        if const_params.is_empty() {
            return;
        }

        let mut visitor = ImplArrayVisitor {
            rng: self.rng,
            const_params,
        };
        visitor.visit_item_impl_mut(i);
    }
}

impl<'a> ArrayConstExprVisitor<'a> {
    fn mutate_binary_expr(&mut self, expr_bin: &mut ExprBinary) {
        use syn::BinOp;
        let ops = [
            BinOp::Add(token::Plus::default()),
            BinOp::Sub(token::Minus::default()),
            BinOp::Mul(token::Star::default()),
            BinOp::Div(token::Slash::default()),
        ];
        let new_op = ops.choose(self.rng).unwrap().clone();
        expr_bin.op = new_op;

        let left_is_path = if let Expr::Path(_) = &*expr_bin.left {
            true
        } else {
            false
        };
        let right_is_path = if let Expr::Path(_) = &*expr_bin.right {
            true
        } else {
            false
        };

        if self.rng.gen_bool(0.5) && left_is_path {
            let mut left_path = match &mut *expr_bin.left {
                Expr::Path(p) => p.clone(),
                _ => return,
            };
            self.adjust_operand_with_offset(&mut left_path, expr_bin);
        } else if right_is_path {
            let mut right_path = match &mut *expr_bin.right {
                Expr::Path(p) => p.clone(),
                _ => return,
            };
            self.adjust_operand_with_offset(&mut right_path, expr_bin);
        }
    }

    fn mutate_single_operand(&mut self, expr_path: &mut ExprPath) {
        if self.rng.gen_bool(0.3) {
            let offset: i32 = self.rng.gen_range(-5..=5);
            if offset == 0 {
                return;
            }
            let ident = expr_path.path.get_ident().cloned().unwrap();
            let offset_expr: Expr = if offset > 0 {
                parse_quote! { #ident + #offset }
            } else {
                let abs_offset = offset.abs();
                parse_quote! { #ident - #abs_offset }
            };
            if let Expr::Binary(bin) = offset_expr {
                *expr_path = ExprPath {
                    attrs: expr_path.attrs.clone(),
                    qself: None,
                    path: syn::Path::from(ident),
                };
            }
        }
    }

    fn adjust_operand_with_offset(&mut self, expr_path: &mut ExprPath, expr_bin: &mut ExprBinary) {
        let offset: i32 = self.rng.gen_range(-2..=2);
        if offset == 0 {
            return;
        }
        let ident = expr_path.path.get_ident().cloned().unwrap();
        let offset_expr: Expr = if offset > 0 {
            parse_quote! { #ident + #offset }
        } else {
            let abs_offset = offset.abs();
            parse_quote! { #ident - #abs_offset }
        };
        if let Expr::Binary(bin) = offset_expr {
            *expr_bin.left = Box::new(Expr::Binary(bin));
        }
    }
}

struct ImplArrayVisitor<'a> {
    rng: &'a mut rand::rngs::ThreadRng,
    const_params: HashSet<Ident>,
}

impl<'a> VisitMut for ImplArrayVisitor<'a> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Array(type_array) = ty {
            if self.contains_const_param(&type_array.len) {
                let mut visitor = ArrayConstExprVisitor {
                    rng: self.rng,
                    changed: false,
                };
                visitor.visit_type_mut(ty);
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
}

impl<'a> ImplArrayVisitor<'a> {
    fn contains_const_param(&self, expr: &Expr) -> bool {
        struct ConstParamChecker<'a> {
            const_params: &'a HashSet<Ident>,
            found: bool,
        }
        impl<'a> Visit<'_> for ConstParamChecker<'a> {
            fn visit_expr_path(&mut self, i: &ExprPath) {
                if let Some(ident) = i.path.get_ident() {
                    if self.const_params.contains(ident) {
                        self.found = true;
                    }
                }
                syn::visit::visit_expr_path(self, i);
            }
        }
        let mut checker = ConstParamChecker {
            const_params: &self.const_params,
            found: false,
        };
        checker.visit_expr(expr);
        checker.found
    }
}