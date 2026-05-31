use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprMacro, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Generic_Argument_Count_Mismatch_And_Incompatible_Literal_233;

impl Mutator for Generic_Argument_Count_Mismatch_And_Incompatible_Literal_233 {
    fn name(&self) -> &str {
        "Generic_Argument_Count_Mismatch_And_Incompatible_Literal_233"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for stmt in &mut func.block.stmts {
                    if let Stmt::Local(local) = stmt {
                        if let Pat::Type(pat_ty) = &mut local.pat {
                            if let Type::Path(type_path) = &mut *pat_ty.ty {
                                if let Some(last_segment) = type_path.path.segments.last_mut() {
                                    if let PathArguments::AngleBracketed(args) = &mut last_segment.arguments {
                                        let current_count = args.args.len();
                                        if current_count >= 1 {
                                            args.args.push(GenericArgument::Type(parse_quote!(u8)));
                                        }
                                    }
                                }
                            }
                        }
                        if let Some(expr) = &mut local.init {
                            if let Expr::Macro(expr_macro) = &mut *expr.expr {
                                if let Some(ident) = expr_macro.mac.path.get_ident() {
                                    if ident == "vec" {
                                        let new_expr = parse_quote! {'β'};
                                        expr.expr = Box::new(new_expr);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}