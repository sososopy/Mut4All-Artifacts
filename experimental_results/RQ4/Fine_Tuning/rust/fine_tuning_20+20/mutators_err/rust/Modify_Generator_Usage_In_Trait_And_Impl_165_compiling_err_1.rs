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

pub struct Modify_Generator_Usage_In_Trait_And_Impl_165;

impl Mutator for Modify_Generator_Usage_In_Trait_And_Impl_165 {
    fn name(&self) -> &str {
        "Modify_Generator_Usage_In_Trait_And_Impl_165"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let mut has_gen = false;
                for item in &mut item_trait.items {
                    if let TraitItem::Type(type_item) = item {
                        if type_item
                            .bounds
                            .iter()
                            .any(|bound| match bound {
                                TypeParamBound::Trait(trait_bound) => trait_bound
                                    .path
                                    .segments
                                    .last()
                                    .map_or(false, |seg| seg.ident == "Generator"),
                                _ => false,
                            })
                        {
                            has_gen = true;
                            type_item.bounds = {
                                let mut bounds = Punctuated::new();
                                bounds.push(TypeParamBound::Trait(TraitBound {
                                    paren_token: None,
                                    modifier: TraitBoundModifier::None,
                                    lifetimes: None,
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: {
                                            let mut segs = Punctuated::new();
                                            segs.push(PathSegment {
                                                ident: Ident::new("Generator", Span::call_site()),
                                                arguments: PathArguments::AngleBracketed(
                                                    AngleBracketedGenericArguments {
                                                        colon2_token: None,
                                                        lt_token: token::Lt::default(),
                                                        args: {
                                                            let mut args = Punctuated::new();
                                                            args.push(GenericArgument::Binding(
                                                                Binding {
                                                                    ident: Ident::new(
                                                                        "Yield",
                                                                        Span::call_site(),
                                                                    ),
                                                                    eq_token: Default::default(),
                                                                    ty: Type::Path(TypePath {
                                                                        qself: None,
                                                                        path: SynPath::from(
                                                                            Ident::new(
                                                                                "u32",
                                                                                Span::call_site(),
                                                                            ),
                                                                        ),
                                                                    }),
                                                                },
                                                            ));
                                                            args.push(GenericArgument::Binding(
                                                                Binding {
                                                                    ident: Ident::new(
                                                                        "Return",
                                                                        Span::call_site(),
                                                                    ),
                                                                    eq_token: Default::default(),
                                                                    ty: Type::Path(TypePath {
                                                                        qself: None,
                                                                        path: SynPath::from(
                                                                            Ident::new(
                                                                                "String",
                                                                                Span::call_site(),
                                                                            ),
                                                                        ),
                                                                    }),
                                                                },
                                                            ));
                                                            args
                                                        },
                                                        gt_token: token::Gt::default(),
                                                    },
                                                ),
                                            }));
                                            segs
                                        },
                                    },
                                }));
                                bounds
                            };
                        }
                    }
                }
                if has_gen {
                    if let Some(gen) = item_trait.generics.params.iter().find_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    }) {
                        item_trait
                            .supertraits
                            .push(parse_quote!(Runnable<#gen>));
                    } else {
                        item_trait.supertraits.push(parse_quote!(Runnable));
                    }
                }
            }
            if let Item::Impl(item_impl) = item {
                let mut has_gen = false;
                for item in &mut item_impl.items {
                    if let ImplItem::Type(type_item) = item {
                        if type_item
                            .ty
                            .as_ref()
                            .is_impl_trait()
                            .map_or(false, |impl_trait| {
                                impl_trait
                                    .bounds
                                    .iter()
                                    .any(|bound| match bound {
                                        TypeParamBound::Trait(trait_bound) => trait_bound
                                            .path
                                            .segments
                                            .last()
                                            .map_or(false, |seg| seg.ident == "Generator"),
                                        _ => false,
                                    })
                            })
                        {
                            has_gen = true;
                            type_item.ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: Default::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: SynPath {
                                            leading_colon: None,
                                            segments: {
                                                let mut segs = Punctuated::new();
                                                segs.push(PathSegment {
                                                    ident: Ident::new(
                                                        "Generator",
                                                        Span::call_site(),
                                                    ),
                                                    arguments: PathArguments::AngleBracketed(
                                                        AngleBracketedGenericArguments {
                                                            colon2_token: None,
                                                            lt_token: token::Lt::default(),
                                                            args: {
                                                                let mut args = Punctuated::new();
                                                                args.push(
                                                                    GenericArgument::Binding(
                                                                        Binding {
                                                                            ident: Ident::new(
                                                                                "Yield",
                                                                                Span::call_site(),
                                                                            ),
                                                                            eq_token:
                                                                                Default::default(),
                                                                            ty: Type::Path(
                                                                                TypePath {
                                                                                    qself: None,
                                                                                    path: SynPath::from(
                                                                                        Ident::new(
                                                                                            "u32",
                                                                                            Span::call_site(),
                                                                                        ),
                                                                                    ),
                                                                                },
                                                                            ),
                                                                        },
                                                                    ),
                                                                );
                                                                args.push(
                                                                    GenericArgument::Binding(
                                                                        Binding {
                                                                            ident: Ident::new(
                                                                                "Return",
                                                                                Span::call_site(),
                                                                            ),
                                                                            eq_token:
                                                                                Default::default(),
                                                                            ty: Type::Path(
                                                                                TypePath {
                                                                                    qself: None,
                                                                                    path: SynPath::from(
                                                                                        Ident::new(
                                                                                            "String",
                                                                                            Span::call_site(),
                                                                                        ),
                                                                                    ),
                                                                                },
                                                                            ),
                                                                        },
                                                                    ),
                                                                );
                                                                args
                                                            },
                                                            gt_token: token::Gt::default(),
                                                        },
                                                    ),
                                                }));
                                                segs
                                            },
                                        },
                                    }));
                                    bounds
                                },
                            }));
                        }
                    }
                }
                if has_gen {
                    if let Some(gen) = item_impl.generics.params.iter().find_map(|param| {
                        if let GenericParam::Type(type_param) = param {
                            Some(type_param.ident.clone())
                        } else {
                            None
                        }
                    }) {
                        item_impl.trait_ = Some((None, parse_quote!(Runnable<#gen>), Default::default()));
                    } else {
                        item_impl.trait_ = Some((None, parse_quote!(Runnable), Default::default()));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets traits and impls that use the Generator trait. It modifies the associated type to yield and return different types, specifically u32 and String, and adjusts the implementation to match. Additionally, it enforces the Runnable supertrait, ensuring that the trait and impl are aligned with this modified generator behavior. The mutation stresses the compiler's handling of generator traits and associated type resolution, potentially uncovering bugs related to type inference and generator logic."
    }
}