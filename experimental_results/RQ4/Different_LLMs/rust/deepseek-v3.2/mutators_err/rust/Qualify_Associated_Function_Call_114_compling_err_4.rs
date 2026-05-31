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

pub struct Qualify_Associated_Function_Call_114;

impl Mutator for Qualify_Associated_Function_Call_114 {
    fn name(&self) -> &str {
        "Qualify_Associated_Function_Call_114"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MutatorVisitor;
        impl VisitMut for MutatorVisitor {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Call(call) = expr {
                    if let Expr::Path(path_expr) = &*call.func {
                        if let Some(last_segment) = path_expr.path.segments.last() {
                            if last_segment.arguments.is_none() {
                                let path_string = path_expr.path.segments.iter().map(|seg| seg.ident.to_string()).collect::<Vec<_>>();
                                if path_string.len() >= 2 && path_string[0] == "Self" && path_string[1].contains("Assoc") {
                                    let trait_name = if path_string.len() > -2 {
                                        path_string[2].clone()
                                    } else {
                                        last_segment.ident.to_string()
                                    };
                                    let qualified_path: Expr = parse_quote! { <Self::Assoc as #trait_name>::#trait_name };
                                    call.func = Box::new(qualified_path);
                                }
                            }
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = MutatorVisitor;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms direct associated function calls (e.g., Self::Assoc::trait_function()) into fully qualified trait calls (<Self::Assoc as Trait>::trait_function()). This targets trait implementations where associated types involve generic const expressions, forcing explicit trait resolution. The change ensures the trait and its associated type are in scope, potentially exposing bugs in the compiler's handling of qualified paths and const-generic-associated type resolution."
    }
}