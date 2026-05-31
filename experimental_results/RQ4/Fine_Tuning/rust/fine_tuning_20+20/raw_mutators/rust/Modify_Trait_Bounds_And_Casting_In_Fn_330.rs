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

pub struct Modify_Trait_Bounds_And_Casting_In_Fn_330;

impl Mutator for Modify_Trait_Bounds_And_Casting_In_Fn_330 {
    fn name(&self) -> &str {
        "Modify_Trait_Bounds_And_Casting_In_Fn_330"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident != "make_dyn_star" {
                    continue;
                }
                let mut new_inputs = Punctuated::new();
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let mut new_bounds = Punctuated::new();
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath::from(Ident::new("PointerLike", Span::call_site())),
                        }));
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath::from(Ident::new("Debug", Span::call_site())),
                        }));
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath::from(Ident::new("Send", Span::call_site())),
                        }));
                        new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site())));
                        let new_ty = Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds: new_bounds,
                        });
                        new_inputs.push(FnArg::Typed(PatType {
                            attrs: pat_type.attrs.clone(),
                            pat: pat_type.pat.clone(),
                            colon_token: pat_type.colon_token,
                            ty: Box::new(new_ty),
                        }));
                    }
                }
                item_fn.sig.inputs = new_inputs;
                item_fn.sig.output = ReturnType::Type(
                    token::RArrow {
                        spans: [Span::call_site()],
                    },
                    Box::new(Type::ImplTrait(TypeImplTrait {
                        impl_token: token::Impl {
                            span: Span::call_site(),
                        },
                        bounds: {
                            let mut bounds = Punctuated::new();
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("PointerLike", Span::call_site())),
                            }));
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("Debug", Span::call_site())),
                            }));
                            bounds.push(TypeParamBound::Trait(TraitBound {
                                paren_token: None,
                                modifier: TraitBoundModifier::None,
                                lifetimes: None,
                                path: SynPath::from(Ident::new("Send", Span::call_site())),
                            }));
                            bounds.push(TypeParamBound::Lifetime(Lifetime::new("'a", Span::call_site())));
                            bounds
                        },
                    })),
                );
                let new_stmt: Stmt = parse_quote! {
                    let casted: &dyn Send = &t;
                };
                let mut new_stmts = Vec::new();
                new_stmts.push(new_stmt);
                for stmt in &item_fn.block.stmts {
                    new_stmts.push(stmt.clone());
                }
                item_fn.block.stmts = new_stmts;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets the function `make_dyn_star` by modifying its trait bounds and type casting. It adds the `Send` trait to both the input and output bounds, and introduces a casting operation to `&dyn Send` within the function body. This transformation is designed to stress-test the compiler's handling of trait bounds and type inference, particularly in the context of trait objects and opaque types."
    }
}