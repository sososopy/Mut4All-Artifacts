use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
};
use syn::{
    BoundLifetimes,
    Expr,
    ExprCall,
    ExprPath,
    File,
    FnArg,
    GenericArgument,
    GenericParam,
    Ident,
    Item,
    ItemFn,
    ItemStruct,
    Lifetime,
    LifetimeParam,
    Local,
    Pat,
    PatType,
    Path as SynPath,
    PathArguments,
    ReturnType,
    Stmt,
    TraitBound,
    TraitBoundModifier,
    Type,
    TypeImplTrait,
    TypeParamBound,
    TypePath,
    parse_quote,
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

pub struct Replace_Type_Alias_With_Concrete_Type_279;

impl Mutator for Replace_Type_Alias_With_Concrete_Type_279 {
    fn name(&self) -> &str {
        "Replace_Type_Alias_With_Concrete_Type_279"
    }

    fn mutate(&self, file: &mut syn::File) {
        let mut type_aliases = Vec::new();

        // Collect type aliases
        for item in &file.items {
            if let Item::Type(type_item) = item {
                type_aliases.push((type_item.ident.clone(), type_item.ty.clone()));
            }
        }

        // Replace type aliases with concrete types
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut return_type) = &mut func.sig.output {
                    for (alias, concrete_type) in &type_aliases {
                        if let Type::Path(path) = &**return_type {
                            if path.path.segments.last().unwrap().ident == *alias {
                                *return_type = concrete_type.clone();
                            }
                        }
                    }
                }

                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        for (alias, concrete_type) in &type_aliases {
                            if let Type::Path(path) = &*pat_type.ty {
                                if path.path.segments.last().unwrap().ident == *alias {
                                    pat_type.ty = concrete_type.clone();
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(method) = impl_item {
                        if let ReturnType::Type(_, ref mut return_type) = &mut method.sig.output {
                            for (alias, concrete_type) in &type_aliases {
                                if let Type::Path(path) = &**return_type {
                                    if path.path.segments.last().unwrap().ident == *alias {
                                        *return_type = concrete_type.clone();
                                    }
                                }
                            }
                        }

                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                for (alias, concrete_type) in &type_aliases {
                                    if let Type::Path(path) = &*pat_type.ty {
                                        if path.path.segments.last().unwrap().ident == *alias {
                                            pat_type.ty = concrete_type.clone();
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
        "This mutation operator replaces type aliases with their concrete types in function and method signatures, testing the compiler's handling of type aliases and their interactions with other language features."
    }
}