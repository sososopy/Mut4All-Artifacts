use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemTrait, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Mutator_NestedHigherRankTraitBounds_448;

impl Mutator for Mutator_NestedHigherRankTraitBounds_448 {
    fn name(&self) -> &str {
        "Mutator_NestedHigherRankTraitBounds_448"
    }
    fn mutate(&self, file: &mut syn::File) {
        let existing_traits: HashSet<String> = file.items.iter()
            .filter_map(|item| if let Item::Trait(trait_item) = item { Some(trait_item.ident.to_string()) } else { None })
            .collect();
        let mut new_traits_to_add = Vec::new();
        
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(impl_trait) = &**return_type {
                        if let Some(trait_bound) = impl_trait.bounds.first() {
                            if let TypeParamBound::Trait(trait_bound) = trait_bound {
                                let trait_name = trait_bound.path.segments.last().unwrap().ident.to_string();
                                let trait_ident = Ident::new(&trait_name, Span::call_site());
                                if !existing_traits.contains(&trait_name) {
                                    let new_trait = parse_quote! {
                                        trait #trait_ident {
                                            type Assoc;
                                        }
                                    };
                                    new_traits_to_add.push(Item::Trait(new_trait));
                                }
                                let trait_exists = existing_traits.contains(&trait_name) || 
                                    new_traits_to_add.iter().any(|t| match t {
                                        Item::Trait(trait_item) => trait_item.ident == trait_ident,
                                        _ => false,
                                    });
                                if !trait_exists {
                                    let new_trait = parse_quote! {
                                        trait #trait_ident {
                                            type Assoc;
                                        }
                                    };
                                    new_traits_to_add.push(Item::Trait(new_trait));
                                }
                                let inner_trait_path = SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: trait_ident.clone(),
                                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: {
                                                    let mut args = Punctuated::new();
                                                    args.push(GenericArgument::Type(Type::Path(TypePath {
                                                        qself: None,
                                                        path: SynPath {
                                                            leading_colon: None,
                                                            segments: {
                                                                let mut segs2 = Punctuated::new();
                                                                segs2.push(PathSegment {
                                                                    ident: Ident::new("T", Span::call_site()),
                                                                    arguments: PathArguments::None,
                                                                });
                                                                segs2
                                                            },
                                                        },
                                                    })));
                                                    args
                                                },
                                                gt_token: token::Gt::default(),
                                            }),
                                        });
                                        segs
                                    },
                                };
                                let inner_trait_bound = TraitBound {
                                    paren_token: Some(Paren::default()),
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: inner_trait_path,
                                };
                                let inner_impl_trait = TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(inner_trait_bound));
                                        bounds
                                    },
                                };
                                let assoc_type = parse_quote! {
                                    type Assoc = impl Trait<T>;
                                };
                                let outer_trait_path = SynPath {
                                    leading_colon: None,
                                    segments: {
                                        let mut segs = Punctuated::new();
                                        segs.push(PathSegment {
                                            ident: trait_ident.clone(),
                                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: {
                                                    let mut args = Punctuated::new();
                                                    args.push(GenericArgument::AssocType(assoc_type));
                                                    args
                                                },
                                                gt_token: token::Gt::default(),
                                            }),
                                        });
                                        segs
                                    },
                                };
                                let outer_trait_bound = TraitBound {
                                    paren_token: Some(Paren::default()),
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: Some(BoundLifetimes {
                                        for_token: token::For::default(),
                                        lt_token: token::Lt::default(),
                                        gt_token: token::Gt::default(),
                                        lifetimes: Punctuated::new(),
                                    }),
                                    path: outer_trait_path,
                                };
                                let new_type = Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(outer_trait_bound));
                                        bounds
                                    },
                                });
                                *return_type = Box::new(new_type);
                            }
                        }
                    }
                }
            }
        }
        
        for new_trait in new_traits_to_add {
            file.items.insert(0, new_trait);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms functions returning `impl Trait` into `impl for<T> Trait<Assoc = impl Trait<T>>` using existing or newly declared traits with associated types. This introduces nested higher-rank trait bounds, creating complex type inference scenarios that stress the compiler's ability to track bound variables and resolve overlapping trait constraints."
    }
}