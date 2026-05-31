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

pub struct Modify_Function_Keywords_In_Trait_Implementations_316;

impl Mutator for Modify_Function_Keywords_In_Trait_Implementations_316 {
    fn name(&self) -> &str {
        "Modify_Function_Keywords_In_Trait_Implementations_316"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if item_impl.trait_.is_none() {
                    continue;
                }
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut has_async = false;
                        let mut has_const = false;
                        let mut has_unsafe = false;
                        let mut has_extern = false;
                        if func.sig.constness.is_some() {
                            has_const = true;
                        }
                        if func.sig.asyncness.is_some() {
                            has_async = true;
                        }
                        if func.sig.unsafety.is_some() {
                            has_unsafe = true;
                        }
                        if func.sig.abi.is_some() {
                            has_extern = true;
                        }
                        if has_async && has_const {
                            func.sig.asyncness = None;
                            func.sig.constness = Some(token::Const::default());
                            return;
                        }
                        if has_const && has_unsafe {
                            func.sig.constness = None;
                            func.sig.unsafety = Some(token::Unsafe::default());
                            return;
                        }
                        if has_const && has_extern {
                            func.sig.constness = None;
                            func.sig.abi = Some(Abi {
                                extern_token: token::Extern::default(),
                                name: None,
                            });
                            return;
                        }
                        if has_async {
                            func.sig.asyncness = None;
                            return;
                        }
                        if has_const {
                            func.sig.constness = None;
                            return;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions within trait implementations by altering their keyword combinations. It swaps or removes `async`, `const`, `unsafe`, and `extern` keywords to test the compiler's handling of these modifications, particularly focusing on combinations that are known to cause ICEs or unexpected behavior in trait contexts."
    }
}