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

pub struct Change_Const_Method_Param_Type_Mismatch_389;

impl Mutator for Change_Const_Method_Param_Type_Mismatch_389 {
    fn name(&self) -> &str {
        "Change_Const_Method_Param_Type_Mismatch_389"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        for item in &file.items {
            if let syn::Item::Trait(item_trait) = item {
                let mut methods = Vec::new();
                for trait_item in &item_trait.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        let method_name = method.sig.ident.clone();
                        let param_types = method.sig.inputs.iter().filter_map(|arg| {
                            if let FnArg::Typed(pat_type) = arg {
                                Some(pat_type.ty.as_ref().clone())
                            } else {
                                None
                            }
                        }).collect::<Vec<_>>();
                        methods.push((method_name, param_types));
                    }
                }
                traits.push((item_trait.ident.clone(), methods));
            }
        }

        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    for (trait_name, methods) in &traits {
                        let trait_path_segments = &trait_path.segments;
                        if let Some(last_segment) = trait_path_segments.last() {
                            if last_segment.ident == *trait_name {
                                for impl_item in &mut item_impl.items {
                                    if let syn::ImplItem::Fn(func) = impl_item {
                                        if func.sig.constness.is_some() {
                                            for (method_name, trait_param_types) in methods {
                                                if func.sig.ident == *method_name {
                                                    if let Some(first_arg) = func.sig.inputs.first_mut() {
                                                        if let FnArg::Typed(pat_type) = first_arg {
                                                            if let Type::Path(type_path) = &*pat_type.ty {
                                                                if let Some(seg) = type_path.path.segments.iter().next() {
                                                                    if seg.ident == "u8" {
                                                                        pat_type.ty = Box::new(parse_quote!(usize));
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
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
        ""
    }
}