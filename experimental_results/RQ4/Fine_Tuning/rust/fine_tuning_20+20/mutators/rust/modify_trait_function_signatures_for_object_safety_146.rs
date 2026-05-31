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

pub struct Modify_Trait_Function_Signatures_For_Object_Safety_146;

impl Mutator for Modify_Trait_Function_Signatures_For_Object_Safety_146 {
    fn name(&self) -> &str {
        "Modify_Trait_Function_Signatures_For_Object_Safety_146"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let mut has_trait_param = false;
                for item in &trait_item.items {
                    if let syn::TraitItem::Fn(method) = item {
                        for input in &method.sig.inputs {
                            if let syn::FnArg::Typed(pat_type) = input {
                                if let syn::Type::TraitObject(_) = *pat_type.ty {
                                    has_trait_param = true;
                                }
                            }
                        }
                        if let syn::ReturnType::Type(_, ty) = &method.sig.output {
                            if let syn::Type::TraitObject(_) = **ty {
                                has_trait_param = true;
                            }
                        }
                    }
                }
                if has_trait_param {
                    let generic_param: syn::GenericParam = parse_quote! { T: ?Sized };
                    trait_item.generics.params.push(generic_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets traits with functions that use trait objects as parameters or return types. It adds a generic parameter `T: ?Sized` to the trait's generics, which is a common pattern for object-safe traits. This transformation stresses the compiler's ability to handle object safety checks and the interaction between trait generics and trait object usage, potentially uncovering issues in trait resolution and object safety analysis."
    }
}