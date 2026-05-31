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

pub struct Replace_Struct_Lifetime_With_Reserved_And_Impl_With_Undefined_192;

impl Mutator for Replace_Struct_Lifetime_With_Reserved_And_Impl_With_Undefined_192 {
    fn name(&self) -> &str {
        "Replace_Struct_Lifetime_With_Reserved_And_Impl_With_Undefined_192"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                if let Some(generics) = &mut struct_item.generics {
                    if generics.params.iter().any(|param| matches!(param, GenericParam::LifetimeParam(_))) {
                        let original_lifetimes: Vec<String> = generics
                            .params
                            .iter()
                            .filter_map(|param| {
                                if let GenericParam::LifetimeParam(lifetime_param) = param {
                                    Some(lifetime_param.lifetime.ident.to_string())
                                } else {
                                    None
                                }
                            })
                            .collect();
                        
                        for param in &mut generics.params {
                            if let GenericParam::LifetimeParam(lifetime_param) = param {
                                lifetime_param.lifetime.ident = Ident::new("static", lifetime_param.lifetime.ident.span());
                            }
                        }
                        
                        let struct_name = &struct_item.ident;
                        for other_item in &mut file.items {
                            if let Item::Impl(impl_item) = other_item {
                                let is_for_struct = if let Some((_, ty, _)) = &impl_item.trait_ {
                                    if let Type::Path(type_path) = &**ty {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            segment.ident == *struct_name
                                        } else {
                                            false
                                        }
                                    } else {
                                        false
                                    }
                                } else {
                                    if let Type::Path(type_path) = &*impl_item.self_ty {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            segment.ident == *struct_name
                                        } else {
                                            false
                                        }
                                    } else {
                                        false
                                    }
                                };
                                
                                if is_for_struct {
                                    let new_lifetime_ident = Ident::new("undefined_lifetime", proc_macro2::Span::call_site());
                                    let new_lifetime = Lifetime::new(&new_lifetime_ident, proc_macro2::Span::call_site());
                                    let new_param = GenericParam::Lifetime(LifetimeParam {
                                        lifetime: new_lifetime,
                                        ..Default::default()
                                    });
                                    impl_item.generics.params.push(new_param);
                                    
                                    for impl_item in &mut impl_item.items {
                                        if let ImplItem::Fn(method) = impl_item {
                                            if let ReturnType::Type(_, ty) = &mut method.sig.output {
                                                if let Type::Reference(ty_ref) = &mut **ty {
                                                    if let Some(lifetime) = &ty_ref.lifetime {
                                                        if original_lifetimes.contains(&lifetime.ident.to_string()) {
                                                            ty_ref.lifetime = Some(Lifetime::new("undefined_lifetime", lifetime.span()));
                                                        }
                                                    }
                                                }
                                            }
                                            
                                            for input in &mut method.sig.inputs {
                                                if let FnArg::Typed(pat_type) = input {
                                                    if let Type::Reference(ty_ref) = &mut *pat_type.ty {
                                                        if let Some(lifetime) = &ty_ref.lifetime {
                                                            if original_lifetimes.contains(&lifetime.ident.to_string()) {
                                                                ty_ref.lifetime = Some(Lifetime::new("undefined_lifetime", lifetime.span()));
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
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces user-defined lifetime parameters in struct definitions with reserved keywords (e.g., 'static), then introduces an impl block with a new, undeclared lifetime parameter. This creates dual errors: invalid lifetime names in the struct and undeclared lifetimes in the impl, increasing the chance of triggering ICE in region error handling."
    }
}