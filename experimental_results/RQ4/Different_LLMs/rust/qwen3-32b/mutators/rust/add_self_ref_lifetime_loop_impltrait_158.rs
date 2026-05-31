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

pub struct Add_Self_Ref_Lifetime_Loop_ImplTrait_158;

impl Mutator for Add_Self_Ref_Lifetime_Loop_ImplTrait_158 {
    fn name(&self) -> &str {
        "Add_Self_Ref_Lifetime_Loop_ImplTrait_158"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(_) = &**return_type {
                        let new_lifetime = syn::LifetimeParam {
                            attrs: Vec::new(),
                            lifetime: syn::Lifetime::new("'a", Span::call_site()),
                            colon_token: Some(Default::default()),
                            bounds: {
                                let mut p = Punctuated::new();
                                p.push(syn::Lifetime::new("'a", Span::call_site()));
                                p
                            },
                        };
                        func.sig.generics.params.push(syn::GenericParam::Lifetime(new_lifetime));
                        let loop_expr: Expr = parse_quote! { loop {} };
                        let stmt = Stmt::Expr(loop_expr, None);
                        func.block.stmts.insert(0, stmt);
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}