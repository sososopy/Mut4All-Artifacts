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

pub struct Inherent_Associated_Type_Generic_Parameter_Mismatch_7;

impl Mutator for Inherent_Associated_Type_Generic_Parameter_Mismatch_7 {
    fn name(&self) -> &str {
        "Inherent_Associated_Type_Generic_Parameter_Mismatch_7"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_indices = Vec::new();
        for (idx, item) in file.items.iter().enumerate() {
            if let syn::Item::Struct(_) = item {
                struct_indices.push(idx);
            }
        }
        
        let mut modifications = Vec::new();
        
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                let mut rng = thread_rng();
                let impl_generics = &item_impl.generics.params;
                let type_params: Vec<Ident> = impl_generics.iter()
                    .filter_map(|param| match param {
                        GenericParam::Type(type_param) => Some(type_param.ident.clone()),
                        _ => None,
                    })
                    .collect();
                
                if type_params.len() == 0 {
                    continue;
                }
                
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Type(assoc_type) = impl_item {
                        let mut visitor = AssocTypeVisitor {
                            type_params: &type_params,
                            rng: &mut rng,
                            changed: false,
                        };
                        visitor.visit_type_mut(&mut assoc_type.ty);
                        
                        if visitor.changed {
                            continue;
                        }
                        
                        if type_params.len() == 1 {
                            let new_param = Ident::new("V", Span::call_site());
                            item_impl.generics.params.push(parse_quote!(#new_param));
                            assoc_type.ty = parse_quote!(#new_param);
                            
                            let target_ident = if let syn::Type::Path(type_path) = &*item_impl.self_ty {
                                type_path.path.segments.last().unwrap().ident.clone()
                            } else {
                                continue;
                            };
                            
                            modifications.push((target_ident, new_param));
                        } else {
                            let current_param = match &assoc_type.ty {
                                syn::Type::Path(type_path) if type_path.path.segments.len() == 1 => {
                                    Some(type_path.path.segments[0].ident.clone())
                                },
                                _ => None,
                            };
                            if let Some(current) = current_param {
                                if type_params.contains(&current) {
                                    let other_params = type_params.iter()
                                        .filter(|p| *p != &current)
                                        .collect::<Vec<_>>();
                                    if other_params.len() > 0 {
                                        let new_param = other_params[rng.gen_range(0..other_params.len())];
                                        assoc_type.ty = parse_quote!(#new_param);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        for (target_ident, new_param) in modifications {
            for idx in &struct_indices {
                if let syn::Item::Struct(struct_item) = &mut file.items[*idx] {
                    if struct_item.ident == target_ident {
                        struct_item.generics.params.push(parse_quote!(#new_param));
                        if let syn::Fields::Named(fields) = &mut struct_item.fields {
                            fields.named.push(parse_quote!(phantom: std::marker::PhantomData<#new_param>));
                        } else if let syn::Fields::Unnamed(fields) = &mut struct_item.fields {
                            fields.unnamed.push(parse_quote!(std::marker::PhantomData<#new_param>));
                        } else {
                            struct_item.fields = syn::Fields::Unnamed(parse_quote!((std::marker::PhantomData<#new_param>)));
                        }
                        break;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets inherent associated type definitions within generic impl blocks. It modifies the associated type definition to use a different generic parameter than intended, creating mismatches. For single generic parameter impls, it adds a new parameter and PhantomData field. For multiple parameters, it swaps the used parameter. This exposes bugs in generic parameter mapping during type resolution and substitution."
    }
}

struct AssocTypeVisitor<'a, R: Rng> {
    type_params: &'a Vec<Ident>,
    rng: &'a mut R,
    changed: bool,
}

impl<'a, R: Rng> VisitMut for AssocTypeVisitor<'a, R> {
    fn visit_type_mut(&mut self, node: &mut Type) {
        if let Type::Path(type_path) = node {
            if type_path.path.segments.len() == 1 {
                let ident = &type_path.path.segments[0].ident;
                if self.type_params.contains(ident) {
                    let other_params = self.type_params.iter()
                        .filter(|p| *p != ident)
                        .collect::<Vec<_>>();
                    if other_params.len() > 0 {
                        let new_param = other_params[self.rng.gen_range(0..other_params.len())];
                        type_path.path.segments[0].ident = new_param.clone();
                        self.changed = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, node);
    }
}