use proc_macro2::Span;
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, 
    token::{Brace, Comma, Paren, Plus}, visit::Visit, visit_mut::VisitMut, *,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath,
};

use crate::mutator::Mutator;

pub struct Modify_Struct_Field_Order_5;

impl Mutator for Modify_Struct_Field_Order_5 {
    fn name(&self) -> &str {
        "Modify_Struct_Field_Order_5"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.fields.len() > 1 {
                    let mut rng = thread_rng();
                    let mut fields: Vec<_> = item_struct.fields.iter().cloned().collect();
                    fields.shuffle(&mut rng);
                    item_struct.fields = syn::Fields::Named(syn::FieldsNamed {
                        brace_token: Brace { span: Span::call_site() },
                        named: Punctuated::from_iter(fields),
                    });
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator reorders the fields within struct definitions. This transformation aims to reveal bugs related to field order assumptions in struct initialization, field access, and type handling. By altering the field order, it challenges the compiler's ability to correctly manage struct layouts and access patterns."
    }
}