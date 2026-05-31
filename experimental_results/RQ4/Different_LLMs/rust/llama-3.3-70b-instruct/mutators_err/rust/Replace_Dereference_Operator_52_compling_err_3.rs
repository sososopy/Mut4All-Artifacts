use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Replace_Dereference_Operator_52;

impl Mutator for Replace_Dereference_Operator_52 {
    fn name(&self) -> &str {
        "Replace_Dereference_Operator_52"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Const(const_item) = item {
                if let Expr::Unary(unary_expr) = &*const_item.expr {
                    if let syn::UnOp::Deref(_) = unary_expr.op {
                        let new_expr = match thread_rng().gen_range(0..3) {
                            0 => parse_quote!(+),
                            1 => parse_quote!(-),
                            2 => parse_quote!(*),
                            _ => unreachable!(),
                        };
                        *const_item.expr = Expr::Unary(syn::ExprUnary {
                            attrs: Vec::new(),
                            op: new_expr,
                            expr: unary_expr.expr.clone(),
                        });
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets constant expressions containing a dereference operator. It replaces the dereference operator with another operator (either +, -, or *) to test the compiler's behavior when handling different operators in constant expressions."
    }
}