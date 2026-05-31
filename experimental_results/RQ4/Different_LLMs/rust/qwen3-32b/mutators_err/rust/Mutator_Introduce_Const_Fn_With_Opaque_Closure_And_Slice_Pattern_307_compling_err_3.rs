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

pub struct Mutator_Introduce_Const_Fn_With_Opaque_Closure_And_Slice_Pattern_307;

impl Mutator for Mutator_Introduce_Const_Fn_With_Opaque_Closure_And_Slice_Pattern_307 {
    fn name(&self) -> &str {
        "Mutator_Introduce_Const_Fn_With_Opaque_Closure_And_Slice_Pattern_307"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }

                // Convert function to const
                func.sig.constness = Some(syn::token::Const {
                    span: Span::call_site(),
                });

                // Set return type to impl ~const Fn()
                let impl_trait = TypeImplTrait {
                    impl_token: token::Impl {
                        span: Span::call_site(),
                    },
                    bounds: {
                        let mut bounds = Punctuated::new();
                        let trait_bound = TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::Tilde,
                            lifetimes: None,
                            path: SynPath::from(Ident::new("Fn", Span::call_site())),
                        };
                        bounds.push(TypeParamBound::Trait(trait_bound));
                        bounds
                    },
                };
                func.sig.output = ReturnType::Type(
                    Default::default(),
                    Box::new(Type::ImplTrait(impl_trait)),
                );

                // Replace body with closure containing slice pattern match
                let new_block = parse_quote! {
                    {
                        const move || {
                            let sl: &[u8] = &[0, 1, 2];
                            match sl {
                                [first, remainder @ ..] => *first,
                                [] => panic!(),
                            }
                        }
                    }
                };
                *func.block = new_block;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}