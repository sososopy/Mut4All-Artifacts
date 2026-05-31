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

pub struct Unresolved_Lifetimes_In_Fn_Signatures_512;

impl Mutator for Unresolved_Lifetimes_In_Fn_Signatures_512 {
    fn name(&self) -> &str {
        "Unresolved_Lifetimes_In_Fn_Signatures_512"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let lifetime_param: LifetimeParam = parse_quote!('a);
                func.sig.generics.params.push(GenericParam::Lifetime(lifetime_param.clone()));

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ref mut type_reference) = *pat_type.ty {
                            type_reference.lifetime = Some(Lifetime::new("'a", Span::call_site()));
                        }
                    }
                }

                if let ReturnType::Type(_, ref mut ty) = func.sig.output {
                    if let Type::Path(TypePath { path, .. }) = &mut **ty {
                        if let Some(last_segment) = path.segments.last_mut() {
                            if let PathArguments::AngleBracketed(ref mut angle_bracketed) = last_segment.arguments {
                                angle_bracketed.args.push(GenericArgument::Lifetime(Lifetime::new("'a", Span::call_site())));
                            } else {
                                last_segment.arguments = PathArguments::AngleBracketed(parse_quote!(<'a>));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces an explicit lifetime parameter `'a` to function signatures that take or return references. It applies the lifetime to input reference types and extends trait objects in return types with the same lifetime. This transformation aims to expose potential borrow checker issues related to lifetime inference and region constraints by creating explicit lifetime relationships."
    }
}