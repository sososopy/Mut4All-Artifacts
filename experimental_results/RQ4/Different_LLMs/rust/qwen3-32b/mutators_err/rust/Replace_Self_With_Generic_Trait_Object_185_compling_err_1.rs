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

pub struct Replace_Self_With_Generic_Trait_Object_185;

impl Mutator for Replace_Self_With_Generic_Trait_Object_185 {
    fn name(&self) -> &str {
        "Replace_Self_With_Generic_Trait_Object_185"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for trait_method in &mut trait_item.items {
                    if let TraitItem::Method(method) = trait_method {
                        if let Some(first_arg) = method.sig.inputs.iter().next() {
                            if let FnArg::Typed(pat_type) = first_arg {
                                if let Pat::Ident(pat_ident) = &*pat_type.pat {
                                    if pat_ident.ident == "self" {
                                        if let Type::Path(type_path) = &*pat_type.ty {
                                            if type_path.path.segments.len() == 1 {
                                                let segment = &type_path.path.segments[0];
                                                if segment.ident == "Self" {
                                                    if let ReturnType::Type(_, return_type) = &method.sig.output {
                                                        let generic_param = parse_quote!(T: Deref<Target = Self> + DispatchFromDyn<T>);
                                                        method.sig.generics.params.push(generic_param);
                                                        if let Some(first_arg_mut) = method.sig.inputs.iter_mut().next() {
                                                            if let FnArg::Typed(pat_type_mut) = first_arg_mut {
                                                                *pat_type_mut.ty = Box::new(parse_quote!(T));
                                                            }
                                                        }
                                                        if let ReturnType::Type(_, return_type_mut) = &mut method.sig.output {
                                                            *return_type_mut = Box::new(parse_quote!(dyn Trait<T>));
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
        ""
    }
}