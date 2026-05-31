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
                // Add new lifetime parameter 'a to the trait
                let new_lifetime = Lifetime::new("'a", Span::call_site());
                let lifetime_param = GenericParam::Lifetime(LifetimeParam {
                    attrs: Vec::new(),
                    lifetime: new_lifetime,
                    colon_token: None,
                    bounds: Punctuated::new(),
                });
                trait_def.generics.params.push(lifetime_param);

                // Process each method in the trait
                for trait_item in &mut trait_def.items {
                    if let syn::TraitItem::Method(method) = trait_item {
                        // Only modify non-async methods
                        if method.sig.asyncness.is_none() {
                            // Mark method as async
                            method.sig.asyncness = Some(token::Async {
                                span: Span::call_site(),
                            });

                            // Add new lifetime parameter 'b to the method
                            let method_lifetime = Lifetime::new("'b", Span::call_site());
                            let method_lifetime_param = GenericParam::Lifetime(LifetimeParam {
                                attrs: Vec::new(),
                                lifetime: method_lifetime.clone(),
                                colon_token: None,
                                bounds: Punctuated::new(),
                            });
                            method.sig.generics.params.push(method_lifetime_param);

                            // Modify receiver to be reference with 'b
                            if let FnArg::Receiver(receiver) = &mut method.sig.inputs[0] {
                                receiver.reference = Some(token::Reference {
                                    span: Span::call_site(),
                                });
                                receiver.and_token = Some(token::And {
                                    span: Span::call_site(),
                                });
                                receiver.lifetime = Some(method_lifetime);
                            }

                            // Set return type to reference with trait's 'a
                            method.sig.output = ReturnType::Type(
                                token::RARROW(Span::call_site()),
                                Box::new(Type::Reference(TypeReference {
                                    and_token: token::Reference {
                                        span: Span::call_site(),
                                    },
                                    lifetime: Some(Lifetime::new("'a", Span::call_site())),
                                    elem_ty: Box::new(Type::Tuple(TypeTuple {
                                        paren_token: token::Paren {
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