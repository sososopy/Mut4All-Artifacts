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

pub struct Insert_Async_Enum_Destructure_46;

impl Mutator for Insert_Async_Enum_Destructure_46 {
    fn name(&self) -> &str {
        "Insert_Async_Enum_Destructure_46"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    continue;
                }
                if let Some(first_param) = func.sig.inputs.first_mut() {
                    if let FnArg::Typed(pat_type) = first_param {
                        if let Pat::Ident(ident_pat) = &*pat_type.pat {
                            let param_name = ident_pat.ident.clone();
                            if let Type::Path(type_path) = &*pat_type.ty {
                                let enum_path = type_path.path.clone();
                                let async_expr = parse_quote! {
                                    async move {
                                        let #enum_path::Variant { field } = #param_name;
                                    }
                                };
                                let new_stmt = Stmt::Expr(async_expr, None);
                                func.block.stmts.insert(0, new_stmt);
                                func.sig.asyncness = Some(syn::token::Async {
                                    span: Span::call_site(),
                                });
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