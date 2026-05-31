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
    Fields,
    FieldsNamed,
};

use crate::mutator::Mutator;

pub struct Add_Recursive_Type_190;

impl Mutator for Add_Recursive_Type_190 {
    fn name(&self) -> &str {
        "Add_Recursive_Type_190"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_name = &item_struct.ident;
                if struct_name == "Main" {
                    continue;
                }
                if let Fields::Named(FieldsNamed { ref mut named, .. }) = item_struct.fields {
                    let recursive_field: syn::Field = parse_quote! {
                        recursive: Option<Box<#struct_name>>
                    };
                    named.push(recursive_field);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator adds a recursive field to each struct, except for a struct named 'Main'. This field is an Option<Box<StructName>>, introducing potential infinite recursion in type definitions. The transformation aims to challenge the compiler's handling of recursive types, which may lead to stack overflows, ICEs, or other deep compiler bugs due to the complexity of type checking and memory management."
    }
}