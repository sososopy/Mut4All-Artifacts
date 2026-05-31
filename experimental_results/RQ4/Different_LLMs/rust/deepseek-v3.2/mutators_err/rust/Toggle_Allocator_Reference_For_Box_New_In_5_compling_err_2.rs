use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, ExprReference, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Toggle_Allocator_Reference_For_Box_New_In_5;

impl Mutator for Toggle_Allocator_Reference_For_Box_New_In_5 {
    fn name(&self) -> &str {
        "Toggle_Allocator_Reference_For_Box_New_In_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = AllocatorRefToggleVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets allocator-aware allocation calls like `Box::new_in`. It toggles the allocator argument between a reference and a non-reference form (e.g., `&Global` to `Global` or vice versa). This mismatch in expected argument type can alter LLVM IR generation and may expose type mismatches during LTO, potentially triggering compiler ICEs related to allocator type representation."
    }
}

struct AllocatorRefToggleVisitor;

impl VisitMut for AllocatorRefToggleVisitor {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Call(call_expr) = expr {
            if let Expr::Path(path_expr) = &*call_expr.func {
                let path_str = quote!(#path_expr).to_string();
                if path_str.contains("Box::new_in") || path_str.contains("new_in") {
                    if call_expr.args.len() >= 2 {
                        let allocator_arg = &mut call_expr.args[1];
                        match allocator_arg {
                            Expr::Reference(ref_expr) => {
                                let inner = ref_expr.expr.clone();
                                *allocator_arg = *inner;
                            }
                            _ => {
                                let new_ref = Expr::Reference(ExprReference {
                                    attrs: Vec::new(),
                                    and_token: token::And::default(),
                                    mutability: None,
                                    expr: Box::new(allocator_arg.clone()),
                                });
                                *allocator_arg = new_ref;
                            }
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_mut(self, expr);
    }
}