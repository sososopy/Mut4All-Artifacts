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

pub struct Modify_Static_Mutable_References_62;

impl Mutator for Modify_Static_Mutable_References_62 {
    fn name(&self) -> &str {
        "Modify_Static_Mutable_References_62"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Path(type_path) = &field.ty {
                        if type_path
                            .path
                            .segments
                            .last()
                            .map_or(false, |segment| segment.ident == "wl_interface")
                        {
                            field.ty = parse_quote!(&'static wl_interface);
                        }
                    }
                }
            }
            if let Item::Static(item_static) = item {
                if let StaticMutability::Mut(_) = item_static.mutability {
                    item_static.mutability = StaticMutability::None;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets static mutable variables, particularly those used in structs. It converts static mutable variables to immutable by removing the `mut` keyword and updates struct fields that reference these variables to use immutable references. This transformation enforces stricter immutability constraints, which can reveal bugs related to improper state modification and improve the robustness of the code by aligning with Rust's safety guarantees."
    }
}