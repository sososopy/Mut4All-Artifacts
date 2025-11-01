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

pub struct Modify_Function_Parameter_Type_339;

impl Mutator for Modify_Function_Parameter_Type_339 {
    fn name(&self) -> &str {
        "Modify_Function_Parameter_Type_339"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(ref mut item_fn) = item {
                let mut new_inputs = Punctuated::new();
                for input in item_fn.sig.inputs.iter() {
                    if let FnArg::Typed(PatType { pat, ty, attrs, colon_token }) = input {
                        if let Type::Reference(type_reference) = &**ty {
                            if let Type::Slice(_) = &*type_reference.elem {
                                let box_type: Type = parse_quote! { Box<[T]> };
                                let new_input = FnArg::Typed(PatType {
                                    attrs: attrs.clone(),
                                    pat: pat.clone(),
                                    colon_token: *colon_token,
                                    ty: Box::new(box_type),
                                });
                                new_inputs.push(new_input);
                            } else {
                                new_inputs.push(input.clone());
                            }
                        } else {
                            new_inputs.push(input.clone());
                        }
                    } else {
                        new_inputs.push(input.clone());
                    }
                }
                item_fn.sig.inputs = new_inputs;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator targets generic functions and modifies function parameter types. Specifically, it transforms parameters that are references to slice types (e.g., &[T]) into boxed slice types (e.g., Box<[T]>). This change is intended to explore differences in behavior between references and boxed slices, particularly in the context of MIR optimizations."
    }
}