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

pub struct Introduce_Invalid_Function_Qualifiers_19;

impl Mutator for Introduce_Invalid_Function_Qualifiers_19 {
    fn name(&self) -> &str {
        "Introduce_Invalid_Function_Qualifiers_19"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, _, _)) = &item_impl.trait_ {
                    for impl_item in &mut item_impl.items {
                        if let syn::ImplItem::Fn(func) = impl_item {
                            let mut qualifiers: Vec<String> = Vec::new();
                            if func.sig.constness.is_some() {
                                qualifiers.push("const".to_string());
                            }
                            if func.sig.asyncness.is_some() {
                                qualifiers.push("async".to_string());
                            }
                            if func.sig.unsafety.is_some() {
                                qualifiers.push("unsafe".to_string());
                            }
                            if func.sig.abi.is_some() {
                                qualifiers.push("extern".to_string());
                            }

                            if qualifiers.len() == 0 {
                                qualifiers.push("const".to_string());
                                qualifiers.push("async".to_string());
                            }

                            qualifiers.shuffle(&mut rng);

                            func.sig.constness = None;
                            func.sig.asyncness = None;
                            func.sig.unsafety = None;
                            func.sig.abi = None;

                            for qualifier in qualifiers {
                                if qualifier == "const" {
                                    func.sig.constness = Some(syn::token::Const::default());
                                } else if qualifier == "async" {
                                    func.sig.asyncness = Some(syn::token::Async::default());
                                } else if qualifier == "unsafe" {
                                    func.sig.unsafety = Some(syn::token::Unsafe::default());
                                } else if qualifier == "extern" {
                                    func.sig.abi = Some(syn::Abi {
                                        extern_token: syn::token::Extern::default(),
                                        name: None,
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait impl blocks and modifies method qualifiers (async, const, unsafe, extern) to create invalid combinations. It reorders existing qualifiers or adds new ones, prioritizing problematic combinations like 'async const fn' or 'const async fn'. This transformation stresses the compiler's qualifier validation and the interaction between const and async in trait implementations, potentially triggering semantic checks and internal compiler errors."
    }
}