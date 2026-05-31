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

pub struct Introduce_Negation_Minimum_Signed_Integer_Literal_200;

impl Mutator for Introduce_Negation_Minimum_Signed_Integer_Literal_200 {
    fn name(&self) -> &str {
        "Introduce_Negation_Minimum_Signed_Integer_Literal_200"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct ReplaceIntLit;
        impl VisitMut for ReplaceIntLit {
            fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
                if let Expr::Lit(expr_lit) = expr {
                    if let Lit::Int(_) = &expr_lit.lit {
                        let new_expr = parse_quote! {
                            -0x8000_0000_0000_0000_0000_0000_0000_0000
                        };
                        *expr = new_expr;
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        let mut visitor = ReplaceIntLit;
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}