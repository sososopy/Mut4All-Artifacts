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

pub struct Static_Slice_Fn_Pointer_Replacement_541;

impl Mutator for Static_Slice_Fn_Pointer_Replacement_541 {
    fn name(&self) -> &str {
        "Static_Slice_Fn_Pointer_Replacement_541"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Reference(type_reference) = &mut field.ty {
                        if let Type::Slice(type_slice) = &*type_reference.elem {
                            if let Type::BareFn(_) = &*type_slice.elem {
                                field.ty = Type::Reference(syn::TypeReference {
                                    and_token: type_reference.and_token,
                                    lifetime: type_reference.lifetime.clone(),
                                    mutability: type_reference.mutability,
                                    elem: Box::new(Type::BareFn(syn::TypeBareFn {
                                        lifetimes: None,
                                        unsafety: None,
                                        abi: None,
                                        fn_token: token::Fn { span: Span::call_site() },
                                        paren_token: syn::token::Paren(Span::call_site()),
                                        inputs: Punctuated::new(),
                                        variadic: None,
                                        output: ReturnType::Default,
                                    })),
                                });
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