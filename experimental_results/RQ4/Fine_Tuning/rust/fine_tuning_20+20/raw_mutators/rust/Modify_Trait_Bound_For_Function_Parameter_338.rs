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

pub struct Modify_Trait_Bound_For_Function_Parameter_338;

impl Mutator for Modify_Trait_Bound_For_Function_Parameter_338 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_For_Function_Parameter_338"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut target_index = None;
                for (index, param) in func.sig.generics.params.iter().enumerate() {
                    if let syn::GenericParam::Type(type_param) = param {
                        if !type_param.bounds.is_empty() {
                            target_index = Some(index);
                            break;
                        }
                    }
                }
                if let Some(index) = target_index {
                    if let syn::GenericParam::Type(type_param) =
                        func.sig.generics.params[index].clone()
                    {
                        let mut new_param = type_param.clone();
                        new_param.bounds.push(parse_quote!(Clone));
                        func.sig.generics.params[index] =
                            syn::GenericParam::Type(new_param.clone());
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        let mut target_index = None;
                        for (index, param) in func.sig.generics.params.iter().enumerate() {
                            if let syn::GenericParam::Type(type_param) = param {
                                if !type_param.bounds.is_empty() {
                                    target_index = Some(index);
                                    break;
                                }
                            }
                        }
                        if let Some(index) = target_index {
                            if let syn::GenericParam::Type(type_param) =
                                func.sig.generics.params[index].clone()
                            {
                                let mut new_param = type_param.clone();
                                new_param.bounds.push(parse_quote!(Clone));
                                func.sig.generics.params[index] =
                                    syn::GenericParam::Type(new_param.clone());
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with generic parameters constrained by a trait bound. It modifies the trait bound of the first such parameter by adding an additional `Clone` constraint. This transformation is designed to introduce potential trait conflicts or resolution challenges, particularly in contexts where the original trait bound may not naturally coexist with `Clone`. By altering the trait requirements, the mutator aims to stress-test the compiler's trait resolution and type inference mechanisms, especially in scenarios involving complex or experimental trait interactions."
    }
}