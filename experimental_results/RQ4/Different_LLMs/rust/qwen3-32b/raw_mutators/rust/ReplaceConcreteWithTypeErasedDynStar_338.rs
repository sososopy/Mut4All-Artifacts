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

pub struct ReplaceConcreteWithTypeErasedDynStar_338;

impl Mutator for ReplaceConcreteWithTypeErasedDynStar_338 {
    fn name(&self) -> &str {
        "ReplaceConcreteWithTypeErasedDynStar_338"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut traits = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                traits.push(trait_item.clone());
            }
        }

        for item in &mut file.items {
            if let Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        let ty = &mut pat_type.ty;
                        if let Type::Path(type_path) = &**ty {
                            if !traits.is_empty() {
                                if let Some(trait_item) = traits.choose(&mut thread_rng()) {
                                    let trait_path = &trait_item.ident;
                                    let dyn_type = parse_quote! { dyn* #trait_path };
                                    *ty = Box::new(dyn_type);
                                }
                            }
                        }
                    }
                }

                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if !traits.is_empty() {
                            if let Some(trait_item) = traits.choose(&mut thread_rng()) {
                                let trait_path = &trait_item.ident;
                                let dyn_type = parse_quote! { dyn* #trait_path };
                                *return_type = Box::new(dyn_type);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces function parameters and return types that use a concrete struct type with a `dyn*` trait object pointer. This transformation introduces type erasure, forcing the compiler to handle dynamic dispatch and trait object casting. It leverages existing trait declarations in the codebase to create potentially invalid type relationships, exposing compiler bugs in type erasure and trait resolution logic."
    }
}