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

pub struct Modify_Static_Mutability_And_Initialization_3;

impl Mutator for Modify_Static_Mutability_And_Initialization_3 {
    fn name(&self) -> &str {
        "Modify_Static_Mutability_And_Initialization_3"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(item_static) = item {
                if let Some(_) = item_static.mutability {
                    item_static.mutability = None;
                    if let Expr::Lit(expr_lit) = &mut *item_static.expr {
                        if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                            let new_value = lit_int.base10_parse::<i32>().unwrap_or(0) + 1;
                            expr_lit.lit = syn::Lit::Int(syn::LitInt::new(
                                &new_value.to_string(),
                                lit_int.span(),
                            ));
                        }
                    }
                } else if let Type::Path(type_path) = &*item_static.ty {
                    if let Some(last_segment) = type_path.path.segments.last() {
                        if last_segment.ident == "OnceLock" || last_segment.ident == "Mutex" {
                            item_static.ty = Box::new(Type::Path(TypePath {
                                qself: None,
                                path: syn::Path::from(Ident::new("u32", Span::call_site())),
                            }));
                            item_static.expr = Box::new(parse_quote! { 0 });
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