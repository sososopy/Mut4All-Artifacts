use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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

pub struct Recursive_Impl_Trait_Return_154;

impl Mutator for Recursive_Impl_Trait_Return_154 {
    fn name(&self) -> &str {
        "Recursive_Impl_Trait_Return_154"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &mut pat_type.ty;
                        let mut target_trait_bound = None;
                        
                        // Check for impl Trait
                        if let Type::ImplTrait(ty_impl) = *ty.clone() {
                            if let Some(first_bound) = ty_impl.bounds.iter().next() {
                                if let TypeParamBound::Trait(trait_bound) = first_bound {
                                    target_trait_bound = Some(trait_bound);
                                }
                            }
                        } else if let Type::TraitObject(ty_trait) = *ty.clone() {
                            if let Some(first_bound) = ty_trait.bounds.iter().next() {
                                if let TypeParamBound::Trait(trait_bound) = first_bound {
                                    target_trait_bound = Some(trait_bound);
                                }
                            }
                        } else if let Type::Path(ty_path) = *ty.clone() {
                            if let Some(ident) = ty_path.path.get_ident() {
                                for param in &func.sig.generics.params {
                                    if let GenericParam::Type(type_param) = param {
                                        if type_param.ident == *ident {
                                            if let Some(first_bound) = type_param.bounds.first() {
                                                if let TypeParamBound::Trait(trait_bound) = first_bound {
                                                    target_trait_bound = Some(trait_bound);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                        if let Some(trait_bound) = target_trait_bound {
                            *ty = Box::new(Type::ImplTrait(TypeImplTrait {
                                impl_token: token::Impl::default(),
                                bounds: {
                                    let mut bounds = Punctuated::new();
                                    bounds.push(TypeParamBound::Trait(TraitBound {
                                        paren_token: None,
                                        modifier: TraitBoundModifier::None,
                                        lifetimes: None,
                                        path: trait_bound.path.clone(),
                                    }));
                                    bounds
                                },
                            }));
                            
                            if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                                *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                                    impl_token: token::Impl::default(),
                                    bounds: {
                                        let mut bounds = Punctuated::new();
                                        bounds.push(TypeParamBound::Trait(TraitBound {
                                            paren_token: None,
                                            modifier: TraitBoundModifier::None,
                                            lifetimes: None,
                                            path: trait_bound.path.clone(),
                                        }));
                                        bounds
                                    },
                                }));
                            }
                            
                            let func_name = &func.sig.ident;
                            let call_expr = parse_quote! { #func_name(#input) };
                            let mut new_block = func.block.clone();
                            new_block.stmts.clear();
                            new_block.stmts.push(Stmt::Expr(call_expr, None));
                            func.block = new_block;
                            break;
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms functions with a parameter implementing a trait and a compatible return type into self-recursive functions returning `impl Trait`. This stresses type inference during opaque type resolution and may expose ICEs related to recursive trait obligations by creating a function that calls itself with the same parameter, leveraging the trait's implementation for type consistency."
    }
}