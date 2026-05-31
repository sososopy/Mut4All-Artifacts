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

pub struct Add_Unused_Lifetime_348;

impl Mutator for Add_Unused_Lifetime_348 {
    fn name(&self) -> &str {
        "Add_Unused_Lifetime_348"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param: LifetimeParam = parse_quote!('unused);
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));

                if let syn::ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(type_path) = &mut **ty {
                        if type_path.qself.is_none() {
                            let segment = type_path.path.segments.last_mut().unwrap();
                            if let PathArguments::AngleBracketed(ref mut args) = segment.arguments {
                                args.args.push(GenericArgument::Lifetime(Lifetime::new("'unused", Span::call_site())));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Struct(item_struct) = item {
                let lifetime_param: LifetimeParam = parse_quote!('unused);
                item_struct.generics.params.push(GenericParam::Lifetime(lifetime_param));
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds an unused lifetime parameter `'unused` to function and struct signatures. This transformation introduces additional complexity in lifetime resolution, potentially leading to ICEs or other bugs related to lifetime inference and variance checking. It aims to test the compiler's robustness in handling redundant or extraneous lifetime parameters."
    }
}