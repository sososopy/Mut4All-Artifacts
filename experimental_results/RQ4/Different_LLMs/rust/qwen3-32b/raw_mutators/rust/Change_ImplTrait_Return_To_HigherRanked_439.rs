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

pub struct Change_ImplTrait_Return_To_HigherRanked_439;

impl Mutator for Change_ImplTrait_Return_To_HigherRanked_439 {
    fn name(&self) -> &str {
        "Change_ImplTrait_Return_To_HigherRanked_439"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                if let syn::ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let syn::Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut bounds = Punctuated::new();
                        let assoc_type_bound = parse_quote! {
                            impl for<T> Bar<T>
                        };
                        let foo_trait_bound = TraitBound {
                            modifier: TraitBoundModifier::For,
                            lifetimes: Some(BoundLifetimes {
                                for_token: Default::default(),
                                lifetimes: Punctuated::from_iter(vec![GenericParam::Type(TypeParam {
                                    attrs: vec![],
                                    ident: Ident::new("T", Span::call_site()),
                                    colon_token: None,
                                    default: None,
                                    constraints: Punctuated::new(),
                                })]),
                            }),
                            path: {
                                let mut path = Path::new();
                                path.leading_colon = None;
                                path.segments = Punctuated::from_iter(vec![PathSegment {
                                    ident: Ident::new("Foo", Span::call_site()),
                                    arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                        colon2_token: None,
                                        lt_token: Default::default(),
                                        args: Punctuated::from_iter(vec![GenericArgument::AssocType(AssocType {
                                            eq_token: Default::default(),
                                            ident: Ident::new("Assoc", Span::call_site()),
                                            gen_args: None,
                                            ty: Box::new(assoc_type_bound),
                                        })]),
                                        gt_token: Default::default(),
                                    }),
                                }]);
                                path
                            },
                        };
                        bounds.push(syn::TypeParamBound::Trait(foo_trait_bound));
                        *return_type = Box::new(syn::Type::ImplTrait(syn::TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}