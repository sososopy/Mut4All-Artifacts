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

pub struct Modify_Trait_Method_Signature_44;

impl Mutator for Modify_Trait_Method_Signature_44 {
    fn name(&self) -> &str {
        "Modify_Trait_Method_Signature_44"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(trait_method) = trait_item {
                        let method_name = &trait_method.sig.ident;
                        let param_type: Type = parse_quote!(&str);
                        let new_param: FnArg = parse_quote!(param1: #param_type);
                        trait_method.sig.inputs.push(new_param);
                        
                        for impl_item in &mut file.items {
                            if let Item::Impl(impl_item) = impl_item {
                                if let Some((_, path, _)) = &impl_item.trait_ {
                                    if path.segments.last().unwrap().ident == trait_item.ident {
                                        for impl_item in &mut impl_item.items {
                                            if let syn::ImplItem::Fn(impl_method) = impl_item {
                                                if impl_method.sig.ident == *method_name {
                                                    let new_param: FnArg = parse_quote!(param1: #param_type);
                                                    impl_method.sig.inputs.push(new_param);
                                                    impl_method.block.stmts.push(parse_quote!(let _ = param1;));
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
        "The mutation operator targets trait method signatures by adding an additional parameter to both the trait and its implementation. This introduces a mismatch between the trait definition and its implementations, which can reveal bugs related to method signature mismatches, especially in scenarios involving specialization and polymorphism."
    }
}