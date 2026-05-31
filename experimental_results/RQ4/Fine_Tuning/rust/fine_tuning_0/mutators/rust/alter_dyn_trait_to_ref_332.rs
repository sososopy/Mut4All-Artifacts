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

pub struct Alter_Dyn_Trait_To_Ref_332;

impl Mutator for Alter_Dyn_Trait_To_Ref_332 {
    fn name(&self) -> &str {
        "Alter_Dyn_Trait_To_Ref_332"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::TraitObject(type_trait_object) = &*pat_type.ty {
                            if type_trait_object.dyn_token.is_some() {
                                pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                    and_token: token::And {
                                        spans: [Span::call_site()],
                                    },
                                    lifetime: None,
                                    mutability: None,
                                    elem: pat_type.ty.clone(),
                                }));
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for input in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::TraitObject(type_trait_object) = &*pat_type.ty {
                                    if type_trait_object.dyn_token.is_some() {
                                        pat_type.ty = Box::new(Type::Reference(syn::TypeReference {
                                            and_token: token::And {
                                                spans: [Span::call_site()],
                                            },
                                            lifetime: None,
                                            mutability: None,
                                            elem: pat_type.ty.clone(),
                                        }));
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
        "The mutation operator alters function parameters from `dyn* Trait` to `&dyn Trait`. This transformation tests the compiler's handling of trait object references and dynamic dispatch, aiming to uncover issues in type compatibility and trait resolution."
    }
}