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
};

use crate::mutator::Mutator;

pub struct Modify_Const_Trait_Impl_With_Lifetime_Annotations_117;

impl Mutator for Modify_Const_Trait_Impl_With_Lifetime_Annotations_117 {
    fn name(&self) -> &str {
        "Modify_Const_Trait_Impl_With_Lifetime_Annotations_117"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.constness.is_some() {
                    if let ReturnType::Type(_, ret_type) = &item_fn.sig.output {
                        if let Type::ImplTrait(type_impl_trait) = &**ret_type {
                            let mut has_fn_trait = false;
                            for bound in &type_impl_trait.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if trait_bound.path.segments.iter().any(|seg| seg.ident == "Fn") {
                                        has_fn_trait = true;
                                        break;
                                    }
                                }
                            }
                            if has_fn_trait {
                                let lifetime = Lifetime::new("'a", Span::call_site());
                                let lifetime_param = LifetimeParam::new(lifetime.clone());
                                let mut new_bounds = type_impl_trait.bounds.clone();
                                new_bounds.push(TypeParamBound::Lifetime(lifetime.clone()));

                                let new_type_impl_trait = TypeImplTrait {
                                    bounds: new_bounds,
                                    ..type_impl_trait.clone()
                                };

                                item_fn.sig.generics.params.push(GenericParam::Lifetime(lifetime_param));
                                item_fn.sig.output = ReturnType::Type(
                                    Default::default(),
                                    Box::new(Type::ImplTrait(new_type_impl_trait)),
                                );

                                if let Some(block) = &mut item_fn.block {
                                    for stmt in &mut block.stmts {
                                        if let Stmt::Local(local) = stmt {
                                            if let Some((_, expr)) = &local.init {
                                                if let Expr::Closure(closure) = &**expr {
                                                    let mut new_closure = closure.clone();
                                                    if let Some(inputs) = &mut new_closure.inputs.first_mut() {
                                                        if let FnArg::Typed(pat_type) = inputs {
                                                            if let Type::Reference(type_ref) = &mut *pat_type.ty {
                                                                type_ref.lifetime = Some(lifetime.clone());
                                                            }
                                                        }
                                                    }
                                                    *stmt = Stmt::Local(Local {
                                                        init: Some((Default::default(), Box::new(Expr::Closure(new_closure)))),
                                                        ..local.clone()
                                                    });
                                                }
                                            }
                                        }
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
        "The mutation operator targets const functions returning opaque types using `impl const Fn()`. It modifies the return type to include lifetime annotations, such as `impl const Fn() + 'a`, and introduces a lifetime parameter in the function signature. This ensures lifetime consistency within the function, especially when dealing with references or borrowed data. Closures within the function are updated to respect the new lifetime requirements."
    }
}