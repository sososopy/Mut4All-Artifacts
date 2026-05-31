use proc_macro2::{Span, TokenStream};
use quote::quote_spanned;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
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
};

use crate::mutator::Mutator;

pub struct Introduce_Variadic_Parameters_24;

impl Mutator for Introduce_Variadic_Parameters_24 {
    fn name(&self) -> &str {
        "Introduce_Variadic_Parameters_24"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.inputs.is_empty() {
                    continue;
                }
                let has_where_clause = func.sig.generics.where_clause.is_some();
                if has_where_clause {
                    let last_input = func.sig.inputs.last().unwrap();
                    let last_span = last_input.span();
                    let variadic_param = syn::FnArg::Typed(PatType {
                        attrs: vec![],
                        pat: Box::new(syn::Pat::Ident(syn::PatIdent {
                            attrs: vec![],
                            by_ref: None,
                            mutability: None,
                            ident: Ident::new("args", last_span),
                            subpat: None,
                        })),
                        colon_token: Default::default(),
                        ty: Box::new(syn::Type::Verbatim(quote_spanned!(last_span=> ...))),
                    });

                    func.sig.inputs.push(variadic_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}