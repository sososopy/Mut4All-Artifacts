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

pub struct Insert_Undeclared_Lifetime_In_Async_Trait_Return_18;

impl Mutator for Insert_Undeclared_Lifetime_In_Async_Trait_Return_18 {
    fn name(&self) -> &str {
        "Insert_Undeclared_Lifetime_In_Async_Trait_Return_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_some() {
                            if let ReturnType::Type(_, ref mut return_type) = method.sig.output {
                                if let Type::Reference(type_ref) = &**return_type {
                                    if type_ref.lifetime.is_none() {
                                        let mut lifetimes_in_scope = HashSet::new();
                                        for param in trait_item.generics.params.iter() {
                                            if let GenericParam::Lifetime(lifetime_param) = param {
                                                lifetimes_in_scope.insert(lifetime_param.lifetime.ident.to_string());
                                            }
                                        }
                                        for param in method.sig.generics.params.iter() {
                                            if let GenericParam::Lifetime(lifetime_param) = param {
                                                lifetimes_in_scope.insert(lifetime_param.lifetime.ident.to_string());
                                            }
                                        }
                                        let mut candidate = 'a';
                                        while lifetimes_in_scope.contains(&candidate.to_string()) {
                                            candidate = (candidate as u8 + 1) as char;
                                            if candidate > 'z' {
                                                candidate = 'a';
                                            }
                                        }
                                        let new_lifetime = Lifetime::new(&format!("'{}", candidate), Span::call_site());
                                        let mut new_type_ref = type_ref.clone();
                                        new_type_ref.lifetime = Some(new_lifetime);
                                        *return_type = Box::new(Type::Reference(new_type_ref));
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
        "The mutation operator targets async trait methods with elided lifetime references in their return types. It inserts an undeclared lifetime parameter into the reference, creating an E0261 error and potentially triggering ICEs during async coroutine analysis. This mimics the bug report scenario where inference variable mismatches occur."
    }
}