use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprIndex, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Max_Slice_Index_Overflow_76;

struct SliceIndexVisitor {
    target_vars: HashSet<String>,
}

impl<'ast> VisitMut for SliceIndexVisitor {
    fn visit_local_mut(&mut self, i: &mut Local) {
        if let Some(init) = &mut i.init {
            if let Expr::Call(call_expr) = &mut *init.expr {
                if let Expr::Path(path_expr) = &*call_expr.func {
                    let segments = &path_expr.path.segments;
                    if segments.len() >= 2 {
                        let second_last = &segments[segments.len() - 2];
                        let last = &segments[segments.len() - 1];
                        if second_last.ident == "slice" && last.ident == "from_raw_parts" {
                            if call_expr.args.len() >= 2 {
                                let second_arg = call_expr.args.get_mut(1).unwrap();
                                *second_arg = Expr::Verbatim(quote! { usize::MAX });
                            }
                            if let Pat::Ident(ident_pat) = &i.pat {
                                self.target_vars.insert(ident_pat.ident.to_string());
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_local_mut(self, i);
    }

    fn visit_expr_index_mut(&mut self, e: &mut ExprIndex) {
        if let Expr::Path(path_expr) = &*e.expr {
            let segments = &path_expr.path.segments;
            if segments.len() == 1 {
                let base_name = segments[0].ident.to_string();
                if self.target_vars.contains(&base_name) {
                    e.index = Box::new(Expr::Verbatim(quote! { usize::MAX - 1 }));
                }
            }
        }
        syn::visit_mut::visit_expr_index_mut(self, e);
    }
}

impl Mutator for Max_Slice_Index_Overflow_76 {
    fn name(&self) -> &str {
        "Max_Slice_Index_Overflow_76"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = SliceIndexVisitor { target_vars: HashSet::new() };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies assignments to variables using `slice::from_raw_parts` and modifies their length parameter to `usize::MAX`, then replaces any index access on those slices with `usize::MAX - 1`. This creates a const context where the slice length is at the maximum value, and the index is one less, likely causing overflow during const evaluation and testing the compiler's handling of unsafe slice indices."
    }
}