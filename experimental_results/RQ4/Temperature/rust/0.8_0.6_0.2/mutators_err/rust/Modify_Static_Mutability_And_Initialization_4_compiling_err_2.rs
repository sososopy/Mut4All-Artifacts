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

pub struct Modify_Static_Mutability_And_Initialization_4;

impl Mutator for Modify_Static_Mutability_And_Initialization_4 {
    fn name(&self) -> &str {
        "Modify_Static_Mutability_And_Initialization_4"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Static(item_static) = item {
                // Toggle mutability
                if let Some(_) = item_static.mutability {
                    // Make immutable and modify initialization
                    item_static.mutability = None;
                    if let syn::Expr::Struct(expr_struct) = &mut *item_static.expr {
                        if let Some(version_expr) = expr_struct.fields.iter_mut().find(|field| field.member.to_string() == "version") {
                            version_expr.expr = parse_quote!(1);
                        }
                    }
                } else {
                    // Introduce a new static and modify references
                    let new_static: syn::Item = parse_quote! {
                        pub static NEW_INTERFACE: wl_interface = wl_interface { version: 2 };
                    };
                    file.items.push(new_static);

                    if let syn::Expr::Struct(expr_struct) = &mut *item_static.expr {
                        if let Some(c_ptr_expr) = expr_struct.fields.iter_mut().find(|field| field.member.to_string() == "c_ptr") {
                            c_ptr_expr.expr = parse_quote!(Some(&NEW_INTERFACE));
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the mutability and initialization of static variables. It converts mutable static variables to immutable ones and adjusts their initialization values. Additionally, it introduces new immutable static variables and redirects references to them. This approach tests the compiler's handling of memory safety, static lifetimes, and mutability constraints, potentially revealing bugs in constant evaluation or static variable handling."
    }
}