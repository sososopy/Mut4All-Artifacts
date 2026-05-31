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

pub struct Add_Opaque_Return_Lifetime_Bound_On_Assoc_Type_295;

impl Mutator for Add_Opaque_Return_Lifetime_Bound_On_Assoc_Type_295 {
    fn name(&self) -> &str {
        "Add_Opaque_Return_Lifetime_Bound_On_Assoc_Type_295"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let TraitItem::Fn(func) = trait_item {
                        if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                let mut new_bounds = Punctuated::new();
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let original_path = &trait_bound.path;
                                        let mut new_path = original_path.clone();
                                        let mut segments = new_path.segments.into_iter();
                                        let first_segment = segments.next().unwrap();
                                        let assoc_type = syn::AssocType {
                                            eq_token: token::Eq::default(),
                                            ident: Ident::new("Type", Span::call_site()),
                                            generics: Some(syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: Punctuated::new(),
                                                gt_token: token::Gt::default(),
                                            }),
                                            ty: syn::Type::Verbatim(quote! { '_' }),
                                        };
                                        let new_segment = syn::PathSegment {
                                            ident: first_segment.ident.clone(),
                                            arguments: syn::PathArguments::AngleBracketed(syn::AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: Punctuated::from_iter(vec![GenericArgument::AssocType(assoc_type)]),
                                                gt_token: token::Gt::default(),
                                            }),
                                        };
                                        let new_segments = Punctuated::from_iter(vec![new_segment]);
                                        let new_path = syn::Path {
                                            leading_colon: original_path.leading_colon,
                                            segments: new_segments,
                                        };
                                        let modified_trait_bound = TraitBound {
                                            path: new_path,
                                            ..trait_bound.clone()
                                        };
                                        new_bounds.push(TypeParamBound::Trait(modified_trait_bound));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                                type_impl_trait.bounds = new_bounds;
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