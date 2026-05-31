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

pub struct Modify_Const_Params_In_Structs_144;

impl Mutator for Modify_Const_Params_In_Structs_144 {
    fn name(&self) -> &str {
        "Modify_Const_Params_In_Structs_144"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                let mut modified = false;
                let mut new_generics = item_struct.generics.clone();
                for param in &mut new_generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Type::Reference(type_ref) = &const_param.ty {
                            if type_ref.lifetime.is_some() {
                                let new_lifetime: Lifetime = parse_quote!('a);
                                const_param.ty = Type::Reference(TypeReference {
                                    and_token: type_ref.and_token,
                                    lifetime: Some(new_lifetime),
                                    mutability: type_ref.mutability,
                                    elem: type_ref.elem.clone(),
                                });
                                modified = true;
                            }
                        }
                    }
                }
                if modified {
                    item_struct.generics = new_generics;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets structs with constant parameters that have a concrete lifetime, such as `'static`. It replaces the concrete lifetime with a generic lifetime parameter `'a`, ensuring that the struct maintains its constraints and lifetime bounds. This transformation explores how the Rust compiler manages lifetime parameters in constant generic contexts, potentially exposing bugs associated with handling these elements."
    }
}