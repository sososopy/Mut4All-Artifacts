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
    token::{Paren, Minus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Index_With_Len_Minus_2_219;

impl Mutator for Replace_Index_With_Len_Minus_2_219 {
    fn name(&self) -> &str {
        "Replace_Index_With_Len_Minus_2_219"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct IndexReplacer;
        impl VisitMut for IndexReplacer {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Index(e) = expr {
                    // Create the len() call on the base expression
                    let len_call = ExprMethodCall {
                        attrs: Vec::new(),
                        receiver: e.expr.clone(),
                        dot_token: token::Dot::default(),
                        method: Ident::new("len", Span::call_site()),
                        turbofish: None,
                        paren_token: Paren::default(),
                        args: Punctuated::new(),
                    };
                    let len_call_expr = Expr::MethodCall(len_call);

                    // Create the literal 2
                    let two_expr = Expr::Lit(ExprLit {
                        attrs: Vec::new(),
                        lit: Lit::Int(syn::LitInt::new("2", Span::call_site())),
                    });

                    // Create the binary expression: len_call - 2
                    let minus_op = BinOp::Sub(syn::token::Minus::default());
                    let binary_expr = Expr::Binary(ExprBinary {
                        attrs: Vec::new(),
                        left: Box::new(len_call_expr),
                        op: minus_op,
                        right: Box::new(two_expr),
                    });

                    // Replace the index with the new expression
                    e.index = Box::new(binary_expr);
                }
                // Continue visiting children
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = IndexReplacer;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces valid array/slice index accesses with `arr[arr.len() - 2]`, creating potential out-of-bounds accesses. This targets index expressions, altering them to depend on the array's length, which can trigger undefined behavior when the length is insufficient. This tests the compiler's handling of bounds checks and MIR optimizations under invalid index conditions."
    }
}