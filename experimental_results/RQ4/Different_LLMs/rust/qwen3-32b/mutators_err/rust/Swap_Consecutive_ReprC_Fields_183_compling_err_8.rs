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
use syn::{MetaList,NestedMeta};

use crate::mutator::Mutator;

pub struct Swap_Consecutive_ReprC_Fields_183;

impl Mutator for Swap_Consecutive_ReprC_Fields_183 {
    fn name(&self) -> &str {
        "Swap_Consecutive_ReprC_Fields_183"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let has_repr_c = struct_item.attrs.iter().any(|attr| {
                    attr.path().is_ident("repr") && 
                    match &attr.meta {
                        syn::Meta::List(meta_list) => {
                            let nested: syn::Result<Vec<NestedMeta>> = syn::parse2(meta_list.tokens.clone());
                            if let Ok(nested) = nested {
                                nested.iter().any(|nested| {
                                    if let NestedMeta::Meta(syn::Meta::Path(path)) = nested {
                                        path.is_ident("C")
                                    } else {
                                        false
                                    }
                                })
                            } else {
                                false
                            }
                        },
                        _ => false,
                    }
                });
                if has_repr_c {
                    match &mut struct_item.fields {
                        syn::Fields::Named(named) => {
                            let fields = &mut named.named;
                            for i in 0..fields.len() - 1 {
                                let field1 = &fields[i];
                                let field2 = &fields[i + 1];
                                let ty1_str = format!("{}", quote!(#field1.ty));
                                let ty2_str = format!("{}", quote!(#field2.ty));
                                if ty1_str != ty2_str {
                                    let temp = fields[i].clone();
                                    fields[i] = fields[i + 1].clone();
                                    fields[i + 1] = temp;
                                    break;
                                }
                            }
                        },
                        syn::Fields::Unnamed(unnamed) => {
                            let fields = &mut unnamed.unnamed;
                            for i in 0..fields.len() - 1 {
                                let field1 = &fields[i];
                                let field2 = &fields[i + 1];
                                let ty1_str = format!("{}", quote!(#field1.ty));
                                let ty2_str = format!("{}", quote!(#field2.ty));
                                if ty1_str != ty2_str {
                                    let temp = fields[i].clone();
                                    fields[i] = fields[i + 1].clone();
                                    fields[i + 1] = temp;
                                    break;
                                }
                            }
                        },
                        _ => {}
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator swaps two consecutive fields in structs with `#[repr(C)]` or equivalent layout attributes, provided the fields have distinct types (as a heuristic for distinct sizes). This disrupts padding and alignment assumptions, potentially causing layout validation errors during transmutation checks. It targets structs containing enum types with variants of differing sizes, challenging the compiler's layout inference and memory model enforcement."
    }
}