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

pub struct Inject_Circular_Type_Alias_401;

impl Mutator for Inject_Circular_Type_Alias_401 {
    fn name(&self) -> &str {
        "Inject_Circular_Type_Alias_401"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Struct(item_struct) = item {
                let struct_ident = &item_struct.ident;
                let circular_type_alias: Item = parse_quote! {
                    type CircularAlias = #struct_ident;
                };
                file.items.push(circular_type_alias);
                
                let circular_type_alias_self: Item = parse_quote! {
                    type CircularSelfAlias = CircularAlias;
                };
                file.items.push(circular_type_alias_self);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator introduces circular type aliases within the file by creating a type alias that refers to a struct and another alias that refers back to the first alias. This transformation is designed to test the compiler's ability to handle circular type references, potentially leading to infinite loops or stack overflows during type resolution."
    }
}