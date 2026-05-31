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

pub struct Change_Trait_Method_To_Async_With_Lifetimes_18;

impl Mutator for Change_Trait_Method_To_Async_With_Lifetimes_18 {
    fn name(&self) -> &str {
        "Change_Trait_Method_To_Async_With_Lifetimes_18"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_def) = item {
                let new_lifetime = Lifetime::new("'a", Span::call_site());
                let lifetime_param = GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: new_lifetime,
                    colon_token: None,
                    bounds: Punctuated::new(),
                });
                trait_def.generics.params.push(lifetime_param);

                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Fn(method) = trait_item {
                        if method.sig.asyncness.is_none() {
                            method.sig.asyncness = Some(token::Async {
                                span: Span::call_site(),
                            });

                            let method_lifetime = Lifetime::new("'b", Span::call_site());
                            let method_lifetime_param = GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: method_lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            });
                            method.sig.generics.params.push(method_lifetime_param);

                            if let FnArg::Receiver(receiver) = &mut method.sig.inputs[0] {
                                receiver.reference = Some(PatReference {
                                    and_token: token::And {
                                        span: Span::call_site(),
                                    },
                                    mutability: None,
                                });
                                receiver.lifetime = Some(method_lifetime);
                            }

                            method.sig.output = ReturnType::Type(
                                token::RArrow(proc_macro2::extra::DelimSpan::new_from_inner(Span::call_site())),
                                Box::new(Type::Reference(TypeReference {
                                    and_token: token::Reference {
                                        span: Span::call_site(),
                                    },
                                    mutability: None,
                                    lifetime: Some(Lifetime::new("'a", Span::call_site())),
                                    elem: Box::new(Type::Tuple(TypeTuple {
                                        paren_token: Paren {
                                            span: Span::call_site(),
                                        },
                                        elems: Punctuated::new(),
                                    })),
                                })),
                            );
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