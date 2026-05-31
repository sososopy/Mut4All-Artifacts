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

pub struct Replace_GAT_Lifetime_With_Static_136;

impl Mutator for Replace_GAT_Lifetime_With_Static_136 {
    fn name(&self) -> &str {
        "Replace_GAT_Lifetime_With_Static_136"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        process_type(&mut pat_type.ty);
                    }
                }
                if let ReturnType::Type(_, return_type) = &mut func.sig.output {
                    process_type(return_type);
                }
            } else if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                process_type(&mut pat_type.ty);
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &mut method.sig.output {
                            process_type(return_type);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces any Generic Associated Type (GAT) usage in method parameters or return types with a 'static lifetime, regardless of whether the GAT's where clause enforces 'static. This transformation introduces potential ICEs during borrow-checking if the GAT's bounds are insufficient, testing the compiler's handling of GAT lifetimes and trait resolution under invalid constraints."
    }
}

fn process_type(ty: &mut Type) {
    match ty {
        Type::Path(type_path) => {
            for segment in &mut type_path.path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        match arg {
                            GenericArgument::Lifetime(lt) => {
                                *lt = Lifetime::new("'static", lt.span());
                            },
                            GenericArgument::Type(ty) => {
                                process_type(ty);
                            },
                            _ => {}
                        }
                    }
                }
            }
        },
        Type::Reference(type_ref) => {
            process_type(&mut *type_ref.elem);
        },
        Type::Ptr(type_ptr) => {
            process_type(&mut *type_ptr.elem);
        },
        Type::Array(type_array) => {
            process_type(&mut *type_array.elem);
        },
        Type::Slice(type_slice) => {
            process_type(&mut *type_slice.elem);
        },
        Type::Tuple(type_tuple) => {
            for ty in &mut type_tuple.elems {
                process_type(ty);
            }
        },
        Type::Paren(type_paren) => {
            process_type(&mut *type_paren.elem);
        },
        Type::BareFn(type_bare_fn) => {
            for input in &mut type_bare_fn.inputs {
                process_type(&mut *input);
            }
            if let ReturnType::Type(_, return_type) = &mut type_bare_fn.output {
                process_type(return_type);
            }
        },
        Type::ImplTrait(type_impl_trait) => {
            for bound in &mut type_impl_trait.bounds {
                if let TypeParamBound::Trait(trait_bound) = bound {
                    if let PathArguments::AngleBracketed(args) = &mut trait_bound.path.segments.last_mut().unwrap().arguments {
                        for arg in &mut args.args {
                            match arg {
                                GenericArgument::Lifetime(lt) => {
                                    *lt = Lifetime::new("'static", lt.span());
                                },
                                GenericArgument::Type(ty) => {
                                    process_type(ty);
                                },
                                _ => {}
                            }
                        }
                    }
                }
            }
        },
        Type::TraitObject(type_trait_object) => {
            for bound in &mut type_trait_object.bounds {
                if let TraitBoundModifier::None = bound.modifier {
                    if let PathArguments::AngleBracketed(args) = &mut bound.path.segments.last_mut().unwrap().arguments {
                        for arg in &mut args.args {
                            match arg {
                                GenericArgument::Lifetime(lt) => {
                                    *lt = Lifetime::new("'static", lt.span());
                                },
                                GenericArgument::Type(ty) => {
                                    process_type(ty);
                                },
                                _ => {}
                            }
                        }
                    }
                }
            }
        },
        Type::Group(type_group) => {
            process_type(&mut *type_group.elem);
        },
        _ => {}
    }
}