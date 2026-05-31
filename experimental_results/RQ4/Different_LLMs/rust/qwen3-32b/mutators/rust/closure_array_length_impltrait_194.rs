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

pub struct Closure_Array_Length_ImplTrait_194;

impl Mutator for Closure_Array_Length_ImplTrait_194 {
    fn name(&self) -> &str {
        "Closure_Array_Length_ImplTrait_194"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                        let mut has_type_with_lifetime_bound = false;
                        let mut target_type = None;
                        for param in &func.sig.generics.params {
                            if let GenericParam::Type(type_param) = param {
                                if type_param.bounds.iter().any(|bound| {
                                    matches!(bound, TypeParamBound::Lifetime(_))
                                }) {
                                    has_type_with_lifetime_bound = true;
                                    target_type = Some(&type_param.ident);
                                    break;
                                }
                            }
                        }
                        if has_type_with_lifetime_bound {
                            if let Some(t_ident) = target_type {
                                let mut new_bounds = Punctuated::new();
                                for bound in &type_impl_trait.bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        let closure: Expr = parse_quote! {
                                            |_: &#t_ident| 0
                                        };
                                        let block: Expr = parse_quote! {
                                            {
                                                #closure;
                                                1
                                            }
                                        };
                                        let array_type: Type = parse_quote! {
                                            [(); #block]
                                        };
                                        let mut new_path = trait_bound.path.clone();
                                        if let Some(segment) = new_path.segments.iter_mut().last() {
                                            let args = parse_quote! {
                                                <Item = #array_type>
                                            };
                                            segment.arguments = PathArguments::AngleBracketed(args);
                                        }
                                        new_bounds.push(TypeParamBound::Trait(TraitBound {
                                            path: new_path,
                                            ..trait_bound.clone()
                                        }));
                                    } else {
                                        new_bounds.push(bound.clone());
                                    }
                                }
                                type_impl_trait.bounds = new_bounds;
                            }
                        }
                    }
                }
            }
            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            if let Type::ImplTrait(type_impl_trait) = &mut **return_type {
                                let mut has_type_with_lifetime_bound = false;
                                let mut target_type = None;
                                for param in &method.sig.generics.params {
                                    if let GenericParam::Type(type_param) = param {
                                        if type_param.bounds.iter().any(|bound| {
                                            matches!(bound, TypeParamBound::Lifetime(_))
                                        }) {
                                            has_type_with_lifetime_bound = true;
                                            target_type = Some(&type_param.ident);
                                            break;
                                        }
                                    }
                                }
                                if has_type_with_lifetime_bound {
                                    if let Some(t_ident) = target_type {
                                        let mut new_bounds = Punctuated::new();
                                        for bound in &type_impl_trait.bounds {
                                            if let TypeParamBound::Trait(trait_bound) = bound {
                                                let closure: Expr = parse_quote! {
                                                    |_: &#t_ident| 0
                                                };
                                                let block: Expr = parse_quote! {
                                                    {
                                                        #closure;
                                                        1
                                                    }
                                                };
                                                let array_type: Type = parse_quote! {
                                                    [(); #block]
                                                };
                                                let mut new_path = trait_bound.path.clone();
                                                if let Some(segment) = new_path.segments.iter_mut().last() {
                                                    let args = parse_quote! {
                                                        <Item = #array_type>
                                                    };
                                                    segment.arguments = PathArguments::AngleBracketed(args);
                                                }
                                                new_bounds.push(TypeParamBound::Trait(TraitBound {
                                                    path: new_path,
                                                    ..trait_bound.clone()
                                                }));
                                            } else {
                                                new_bounds.push(bound.clone());
                                            }
                                        }
                                        type_impl_trait.bounds = new_bounds;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies functions returning `impl Trait` by adding an `Item = [(); { |_: &T| 0; 1 }]` associated type to the first trait bound in the return type. This introduces a nested closure within an array-length expression inside an opaque return type, stressing the compiler's handling of lifetimes in complex expressions and potentially exposing resolution bugs in trait object bounds."
    }
}