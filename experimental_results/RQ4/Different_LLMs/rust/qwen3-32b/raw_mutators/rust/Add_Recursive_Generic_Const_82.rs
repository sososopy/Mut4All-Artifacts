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

pub struct Add_Recursive_Generic_Const_82;

impl Mutator for Add_Recursive_Generic_Const_82 {
    fn name(&self) -> &str {
        "Add_Recursive_Generic_Const_82"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let has_const = item_struct.generics.params.iter().any(|param| {
                    matches!(param, GenericParam::Const(_))
                });
                if has_const {
                    let struct_name = &item_struct.ident;
                    let new_param = parse_quote! {
                        const #struct_name: #struct_name = #struct_name
                    };
                    item_struct.generics.params.push(new_param);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces a recursive generic const parameter with the same identifier as the struct's name, creating a cyclic type reference. The new parameter's type is the struct's name, and its default value references the struct's name directly. This triggers compiler validation errors or ICE by forcing a recursive substitution cycle in const generics, testing the compiler's ability to handle self-referential type definitions."
    }
}