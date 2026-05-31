use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemConst, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Introduce_Mutable_Ref_To_Static_In_Const_60;

impl Mutator for Introduce_Mutable_Ref_To_Static_In_Const_60 {
    fn name(&self) -> &str {
        "Introduce_Mutable_Ref_To_Static_In_Const_60"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut static_items = Vec::new();
        let mut has_const = false;
        for item in &file.items {
            if let Item::Static(item_static) = item {
                if let Type::Path(type_path) = &*item_static.ty {
                    let type_name = type_path.path.segments.last().map(|seg| seg.ident.to_string());
                    if let Some(name) = type_name {
                        if name == "i32" || name == "u32" || name == "i64" || name == "u64" ||
                           name == "i16" || name == "u16" || name == "i8" || name == "u8" ||
                           name == "f32" || name == "f64" || name == "bool" || name == "char" {
                            static_items.push((item_static.ident.clone(), item_static.ty.clone()));
                        }
                    }
                }
            }
            if let Item::Const(_) = item {
                has_const = true;
            }
        }
        if static_items.is_empty() || !has_const {
            return;
        }
        let mut wrapper_exists = false;
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.ident == "MutRefWrapper" {
                    wrapper_exists = true;
                    break;
                }
            }
        }
        if !wrapper_exists {
            let wrapper_struct: ItemStruct = parse_quote! {
                struct MutRefWrapper {
                    x: &'static std::cell::UnsafeCell<i32>
                }
            };
            file.items.push(Item::Struct(wrapper_struct));
        }
        let static_info = static_items.first().unwrap();
        let static_name = &static_info.0;
        let static_type = &static_info.1;
        let new_const: ItemConst = parse_quote! {
            const MUTABLE_REF_CONST: MutRefWrapper = MutRefWrapper {
                x: &mut *(&#static_name as *const i32 as *mut i32)
            };
        };
        file.items.push(Item::Const(new_const));
        let mut has_main = false;
        for item in &file.items {
            if let Item::Fn(item_fn) = item {
                if item_fn.sig.ident == "main" {
                    has_main = true;
                    break;
                }
            }
        }
        if !has_main {
            let main_fn: ItemFn = parse_quote! {
                fn main() {}
            };
            file.items.push(Item::Fn(main_fn));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces mutable references to static memory within const contexts using raw pointer casts and interior mutability. It identifies static items with primitive types and at least one const item in the seed program. It adds a struct MutRefWrapper containing a reference to an UnsafeCell of the static's type, then creates a const MUTABLE_REF_CONST that initializes this wrapper by casting a reference to the static to a mutable pointer and dereferencing to a mutable reference. This mutation targets const-eval validity checks around mutable references and interior mutability in constants, potentially triggering compiler bugs related to const safety and mutable reference creation."
    }
}