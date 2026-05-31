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

pub struct Modify_Array_Index_To_Out_Of_Bounds_219;

impl Mutator for Modify_Array_Index_To_Out_Of_Bounds_219 {
    fn name(&self) -> &str {
        "Modify_Array_Index_To_Out_Of_Bounds_219"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut visitor = ArrayIndexVisitor;
                visitor.visit_item_fn_mut(item_fn);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets array indexing operations and modifies the index to potentially cause out-of-bounds access. This transformation aims to provoke compiler errors or unexpected behavior by altering index expressions to exceed array bounds."
    }
}

struct ArrayIndexVisitor;

impl VisitMut for ArrayIndexVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Index(expr_index) = expr {
            if let Expr::Path(ref path) = *expr_index.expr {
                if let Some(segment) = path.path.segments.last() {
                    if segment.ident == "len" {
                        return;
                    }
                }
            }
            let new_index: Expr = parse_quote!(arr.len());
            expr_index.index = Box::new(new_index);
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}