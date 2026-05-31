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

pub struct Replace_Const_Generic_With_FnPtr_Default_49;

impl Mutator for Replace_Const_Generic_With_FnPtr_Default_49 {
    fn name(&self) -> &str {
        "Replace_Const_Generic_With_FnPtr_Default_49"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(s) = item {
                self.process_generics(&mut s.generics);
            } else if let syn::Item::Enum(e) = item {
                self.process_generics(&mut e.generics);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}

impl Replace_Const_Generic_With_FnPtr_Default_49 {
    fn process_generics(&self, generics: &mut syn::Generics) {
        let has_const = generics.params.iter().any(|p| matches!(p, GenericParam::Const(_)));
        if has_const {
            for param in &mut generics.params {
                if let GenericParam::Const(c) = param {
                    c.ty = parse_quote! { fn() };
                    if c.eq_token.is_none() {
                        c.eq_token = Some(token::Eq {
                            spans: [Span::call_site()],
                        });
                    }
                    if c.default.is_none() {
                        c.default = Some(parse_quote! {{ || {} }});
                    }
                }
            }
        } else {
            let new_param = parse_quote! {
                const FN: fn() = { || {} }
            };
            generics.params.push(new_param);
        }
    }
}