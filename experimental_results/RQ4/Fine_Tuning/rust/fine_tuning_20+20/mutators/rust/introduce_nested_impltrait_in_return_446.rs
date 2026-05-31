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

pub struct Introduce_Nested_ImplTrait_In_Return_446;

impl Mutator for Introduce_Nested_ImplTrait_In_Return_446 {
    fn name(&self) -> &str {
        "Introduce_Nested_ImplTrait_In_Return_446"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        let mut new_bounds = Punctuated::new();
                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                            paren_token: None,
                            modifier: TraitBoundModifier::None,
                            lifetimes: None,
                            path: SynPath {
                                leading_colon: None,
                                segments: {
                                    let mut segments = Punctuated::new();
                                    segments.push(PathSegment {
                                        ident: Ident::new("IntoIterator", Span::call_site()),
                                        arguments: PathArguments::AngleBracketed(
                                            AngleBracketedGenericArguments {
                                                colon2_token: None,
                                                lt_token: token::Lt::default(),
                                                args: {
                                                    let mut args = Punctuated::new();
                                                    args.push(GenericArgument::Type(
                                                        Type::ImplTrait(type_impl_trait.clone()),
                                                    ));
                                                    args
                                                },
                                                gt_token: token::Gt::default(),
                                            },
                                        ),
                                    });
                                    segments
                                },
                            },
                        }));
                        *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                            impl_token: token::Impl {
                                span: Span::call_site(),
                            },
                            bounds: new_bounds,
                        }));
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions with `impl Trait` return types and transforms them into nested `impl Trait` structures by wrapping the original `impl Trait` in another `impl IntoIterator`. It leverages the complex trait interactions that led to bug #446, aiming to stress-test the compiler's handling of nested opaque types and trait resolution."
    }
}