use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *,
};
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

pub struct Replace_Trait_Bounds_With_Const_Generics_405;

impl Mutator for Replace_Trait_Bounds_With_Const_Generics_405 {
    fn name(&self) -> &str {
        "Replace_Trait_Bounds_With_Const_Generics_405"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for arg in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = arg {
                        if let Type::TraitObject(bound) = &*pat_type.ty {
                            let new_bound = self.replace_trait_bound_with_const_generic(bound);
                            pat_type.ty = Box::new(Type::TraitObject(new_bound));
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::TraitObject(bound) = &**return_type {
                        let new_bound = self.replace_trait_bound_with_const_generic(bound);
                        *return_type = Box::new(Type::TraitObject(new_bound));
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        for arg in &mut func.sig.inputs {
                            if let FnArg::Typed(pat_type) = arg {
                                if let Type::TraitObject(bound) = &*pat_type.ty {
                                    let new_bound = self.replace_trait_bound_with_const_generic(bound);
                                    pat_type.ty = Box::new(Type::TraitObject(new_bound));
                                }
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::TraitObject(bound) = &**return_type {
                                let new_bound = self.replace_trait_bound_with_const_generic(bound);
                                *return_type = Box::new(Type::TraitObject(new_bound));
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces trait bounds in trait object types with const generic parameters. This transformation aims to test the compiler's handling of const generics and trait bounds, potentially triggering bugs related to type resolution and inference."
    }
}

impl Replace_Trait_Bounds_With_Const_Generics_405 {
    fn replace_trait_bound_with_const_generic(&self, bound: &TypeTraitObject) -> TypeTraitObject {
        let mut new_bound = bound.clone();
        for bound in &mut new_bound.bounds {
            if let TypeParamBound::Trait(trait_bound) = bound {
                let const_generic_param = self.generate_const_generic_param();
                let new_trait_bound = TraitBound {
                    paren_token: None,
                    modifier: trait_bound.modifier.clone(),
                    lifetimes: trait_bound.lifetimes.clone(),
                    path: self.replace_path_with_const_generic(trait_bound.path.clone(), const_generic_param),
                };
                *bound = TypeParamBound::Trait(new_trait_bound);
            }
        }
        new_bound
    }

    fn generate_const_generic_param(&self) -> Ident {
        let mut rng = thread_rng();
        let const_name = format!("CONST_{}", rng.gen::<u32>());
        Ident::new(&const_name, Span::call_site())
    }

    fn replace_path_with_const_generic(&self, path: SynPath, const_generic_param: Ident) -> SynPath {
        let mut new_path = path.clone();
        new_path.segments.push(PathSegment {
            ident: const_generic_param,
            arguments: PathArguments::None,
        });
        new_path
    }
}