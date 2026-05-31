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

pub struct Modify_Trait_And_Function_Bound_Syntax_442;

impl Mutator for Modify_Trait_And_Function_Bound_Syntax_442 {
    fn name(&self) -> &str {
        "Modify_Trait_And_Function_Bound_Syntax_442"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_impl_trait_return = false;
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &**return_type {
                        has_impl_trait_return = true;
                        let mut new_bounds = type_impl_trait.bounds.clone();
                        new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                        item_fn.sig.output = ReturnType::Type(
                            token::RArrow {
                                spans: [Span::call_site(), Span::call_site()],
                            },
                            Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: token::Impl {
                                    span: Span::call_site(),
                                },
                                bounds: new_bounds,
                            })),
                        );
                    }
                }
                if has_impl_trait_return {
                    let mut new_generics = item_fn.sig.generics.clone();
                    for param in &mut new_generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if type_param.bounds.is_empty() {
                                continue;
                            }
                            let mut new_bounds = type_param.bounds.clone();
                            new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                            type_param.bounds = new_bounds;
                        }
                    }
                    item_fn.sig.generics = new_generics;
                }
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(method) = impl_item {
                        let mut has_impl_trait_return = false;
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &**return_type {
                                has_impl_trait_return = true;
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                                method.sig.output = ReturnType::Type(
                                    token::RArrow {
                                        spans: [Span::call_site(), Span::call_site()],
                                    },
                                    Box::new(Type::ImplTrait(TypeImplTrait {
                                        impl_token: token::Impl {
                                            span: Span::call_site(),
                                        },
                                        bounds: new_bounds,
                                    }),
                                ));
                            }
                        }
                        if has_impl_trait_return {
                            let mut new_generics = method.sig.generics.clone();
                            for param in &mut new_generics.params {
                                if let GenericParam::Type(type_param) = param {
                                    if type_param.bounds.is_empty() {
                                        continue;
                                    }
                                    let mut new_bounds = type_param.bounds.clone();
                                    new_bounds.push(TypeParamBound::Lifetime(Lifetime::new("'static", Span::call_site())));
                                    type_param.bounds = new_bounds;
                                }
                            }
                            method.sig.generics = new_generics;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions and methods with `impl Trait` return types. It modifies the return type by adding a `'static` lifetime bound to the `impl Trait`. Additionally, it updates any type parameters with existing bounds to include a `'static` lifetime bound. This transformation increases the complexity of trait and lifetime constraints, potentially leading to inference challenges and edge cases in the compiler's trait resolution and lifetime analysis."
    }
}