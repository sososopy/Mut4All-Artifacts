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
    ItemTrait, TraitItem,
};

use crate::mutator::Mutator;

pub struct Replace_Extern_C_Return_With_Impl_Trait_201;

impl Mutator for Replace_Extern_C_Return_With_Impl_Trait_201 {
    fn name(&self) -> &str {
        "Replace_Extern_C_Return_With_Impl_Trait_201"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut dummy_trait_defined = false;
        let mut dummy_trait: Option<Item> = None;

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Fn(method) = trait_item {
                        if method.sig.abi.is_some() && method.sig.abi.as_ref().unwrap().name.as_ref().map(|n| n.value()) == Some("C".to_string()) {
                            if let ReturnType::Type(_, ref mut ty) = method.sig.output {
                                if !dummy_trait_defined {
                                    dummy_trait = Some(parse_quote! {
                                        trait DummyTrait {
                                            fn dummy(&self) -> i32;
                                        }
                                    });
                                    dummy_trait_defined = true;
                                }
                                *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl {
                                        span: Span::call_site(),
                                    },
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
                                                        ident: Ident::new("DummyTrait", Span::call_site()),
                                                        arguments: PathArguments::None,
                                                    });
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
                }
            }
        }

        if let Some(dummy_trait) = dummy_trait {
            file.items.insert(0, dummy_trait);
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}