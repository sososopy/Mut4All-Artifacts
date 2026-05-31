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

pub struct Substitute_Type_Parameter_With_Trait_Object_222;

impl Mutator for Substitute_Type_Parameter_With_Trait_Object_222 {
    fn name(&self) -> &str {
        "Substitute_Type_Parameter_With_Trait_Object_222"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(item_impl) = item {
                if let Some(generics) = &item_impl.generics {
                    for param in &generics.params {
                        if let GenericParam::Type(type_param) = param {
                            for bound in &type_param.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    let trait_path = &trait_bound.path;
                                    let trait_ident = trait_path.segments.last().unwrap().ident.clone();
                                    let trait_obj = quote! {
                                        Box<dyn #trait_ident>
                                    };
                                    for impl_item in &mut item_impl.items {
                                        if let syn::ImplItem::Fn(func) = impl_item {
                                            for arg in &mut func.sig.inputs {
                                                if let FnArg::Typed(pat_type) = arg {
                                                    if let Type::Path(type_path) = &*pat_type.ty {
                                                        if type_path.path.segments.last().unwrap().ident == type_param.ident {
                                                            pat_type.ty = trait_obj.clone();
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
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator substitutes type parameters with trait objects in impl blocks. It replaces type parameters with Box<dyn Trait> to test the compiler's handling of trait objects and type parameters."
    }
}